
#pragma once

#include <editor/editorexport.hh>
#include <cobalt/core/csexception.hh>
#include <filesystem>
#include <string>

namespace cs
{
class :ResourceLocator;
}

namespace asset::model
{
	class ModelTransaction;

    class AlterFSException : public cs::Exception
	{
	public:
        AlterFSException(const std::string &message);
	};




	class SecureFS
	{
	public:
		SecureFS(ModelTransaction &tr);

		void Move(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
    void Delete(const cs::ResourceLocator &locator);

	private:

		void Move(std::filesystem::path &oldPath, std::filesystem::path &newPath);
		void MoveFile(std::filesystem::path &oldPath, std::filesystem::path &newPath);
		void MoveDirectory(std::filesystem::path &oldPath, std::filesystem::path &newPath);

    void Delete(std::filesystem::path &path);
    void DeleteFile(std::filesystem::path &path);
    void DeleteDirectory(std::filesystem::path &path);

    std::filesystem::path GetTempFile();

		ModelTransaction & m_tr;
	};
}
