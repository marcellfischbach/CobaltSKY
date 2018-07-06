
#include <assetmodel/sync/securefs.hh>
#include <assetmodel/modeltransaction.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>

namespace asset::model
{

	SecureFS::SecureFS(ModelTransaction &tr)
		: m_tr(tr)
	{

	}


	void SecureFS::Move(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
		std::filesystem::path oldPath(csVFS::Get()->GetAbsolutePath(oldLocator, csVFS::CheckExistence));
		std::filesystem::path newPath(csVFS::Get()->GetAbsolutePath(newLocator, csVFS::DontCheckExistence));
		Move(oldPath, newPath);
	}

	void SecureFS::Move(std::filesystem::path &oldPath, std::filesystem::path &newPath)
	{
		if (!std::filesystem::exists(oldPath))
		{
			throw AlterFSException("Source " + oldPath.generic_string() + " does not exsist");
		}
		if (std::filesystem::exists(newPath) && std::filesystem::is_regular_file(newPath))
		{
			throw AlterFSException("Destination " + newPath.generic_string() + " already exists.");
		}

		if (std::filesystem::is_directory(oldPath))
		{
			MoveDirectory(oldPath, newPath);
		}
		else if (std::filesystem::is_regular_file(oldPath))
		{
			MoveFile(oldPath, newPath);
		}
	}

	void SecureFS::MoveFile(std::filesystem::path &oldPath, std::filesystem::path &newPath)
	{
		if (!std::filesystem::exists(oldPath))
		{
			throw AlterFSException("Source " + oldPath.generic_string() + " does not exsist");
		}
		if (!std::filesystem::is_regular_file(oldPath))
		{
			throw AlterFSException("Source " + oldPath.generic_string() + " is not a regular file");
		}
		if (std::filesystem::exists(newPath))
		{
			throw AlterFSException("Destination " + newPath.generic_string() + " already exists");
		}

		try
		{
			if (!std::filesystem::copy_file(oldPath, newPath))
			{
				throw AlterFSException("Unable to copy " + oldPath.generic_string() + " to " + newPath.generic_string());
			}
		}
		catch (const std::exception &e)
		{
			throw AlterFSException("Unable to copy " + oldPath.generic_string() + " to " + newPath.generic_string());
		}
	}

	class SecureFSCreateDirectory : public ModelTransaction::iCallback
	{
	public:
		SecureFSCreateDirectory(std::filesystem::path &oldPath, std::filesystem::path &newPath)
			: ModelTransaction::iCallback()
			, m_oldPath(oldPath)
			, m_validNewPath(true)
			, m_newPath(newPath)
		{

		}
		SecureFSCreateDirectory(std::filesystem::path &oldPath)
			: ModelTransaction::iCallback()
			, m_oldPath(oldPath)
			, m_validNewPath(false)
		{

		}

		virtual ~SecureFSCreateDirectory()
		{

		}

		void OnCommit()
		{
			try
			{
				std::filesystem::remove_all(m_oldPath);
			}
			catch (const std::exception &e)
			{

			}
		}
		void OnRollback()
		{
			if (m_validNewPath)
			{
				try
				{
					std::filesystem::remove_all(m_newPath);
				}
				catch (const std::exception &e)
				{

				}
			}
		}

	private:
		std::filesystem::path m_oldPath;
		bool m_validNewPath;
		std::filesystem::path m_newPath;
	};

	void SecureFS::MoveDirectory(std::filesystem::path &oldPath, std::filesystem::path &newPath)
	{
		if (!std::filesystem::exists(oldPath))
		{
			throw AlterFSException("Source " + oldPath.generic_string() + " does not exsist");
		}
		if (!std::filesystem::is_directory(oldPath))
		{
			throw AlterFSException("Source " + oldPath.generic_string() + " is not a directory");
		}
		if (std::filesystem::exists(newPath) && std::filesystem::is_regular_file(newPath))
		{
			throw AlterFSException("Destination " + newPath.generic_string() + " already exists");
		}

		if (!std::filesystem::exists(newPath))
		{
			try
			{
				if (!std::filesystem::create_directories(newPath))
				{
					throw AlterFSException("Unable to create " + newPath.generic_string() + "  directory");
				}
			}
			catch (const std::exception &e)
			{
				throw AlterFSException("Unable to create " + newPath.generic_string() + "  directory");
			}

			m_tr.Attach(new SecureFSCreateDirectory(oldPath, newPath));
		}
		else
		{
			m_tr.Attach(new SecureFSCreateDirectory(oldPath));
		}

		for (auto &e : std::filesystem::directory_iterator(oldPath))
		{
			std::filesystem::path oldEntryPath = e.path();
			std::filesystem::path newEntryPath(newPath.generic_string() + "/" + oldEntryPath.filename().generic_string());
			Move(oldEntryPath, newEntryPath);
		}
	}

}