

#pragma once


#include <filesystem>

namespace cs::editor::model
{


class Transaction;
class SecureFS
{
public:
  SecureFS();

  void Rename(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);
  void Delete(const std::filesystem::path &path, bool forceDelete, Transaction &tx);

private:

  void RenameFile(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);
  void RenameDirectory(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);

  void DeleteFile(const std::filesystem::path &path, bool forceDelete, Transaction &tx);
  void DeleteDirectory(const std::filesystem::path &path, bool forceDelete, Transaction &tx);

  void ValidateTempDirectory();

  std::filesystem::path GenerateUniqueTempPath();
  

};


}