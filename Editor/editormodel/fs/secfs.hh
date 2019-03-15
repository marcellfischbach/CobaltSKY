

#pragma once

#include <editormodel/editormodelexport.hh>
#include <filesystem>

namespace cs::editor::model
{


class Transaction;
class EDITOR_MODEL_API SecureFS
{
public:
  SecureFS();

  void CreateFolder(const std::filesystem::path &path, Transaction &tx);
  void Rename(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);
  void Delete(const std::filesystem::path &path, Transaction &tx);
  void Move(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);

private:

  void RenameFile(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);
  void RenameDirectory(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);

  void DeleteFile(const std::filesystem::path &path, Transaction &tx);
  void DeleteDirectory(const std::filesystem::path &path, Transaction &tx);

  void MoveFile(const std::filesystem::path &filePath, const std::filesystem::path &folderPath, Transaction &tx);

  void ValidateTempDirectory();

  std::filesystem::path GenerateUniqueTempPath();
  

};


}