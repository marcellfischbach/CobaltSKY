
#pragma once

#include <exception>
#include <filesystem>
#include <string>

class csResourceLocator;

namespace asset::model
{
	class ModelTransaction;

	class AlterFSException : public std::exception
	{
	public:
		AlterFSException(const std::string &message)
			: std::exception(message.c_str())
		{

		}
	};




	class SecureFS
	{
	public:
		SecureFS(ModelTransaction &tr);

		void Move(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);

	private:

		void Move(std::filesystem::path &oldPath, std::filesystem::path &newPath);
		void MoveFile(std::filesystem::path &oldPath, std::filesystem::path &newPath);
		void MoveDirectory(std::filesystem::path &oldPath, std::filesystem::path &newPath);
		ModelTransaction & m_tr;
	};
}