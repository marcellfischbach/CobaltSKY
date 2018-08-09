
#pragma once

#include <editor/editorexport.hh>
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
    void Delete(const csResourceLocator &locator);

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