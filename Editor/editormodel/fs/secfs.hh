

#pragma once


#include <filesystem>
#include <editormodel/fs/fsmodificationexception.hh>

namespace cs::editor::model
{


class Transaction;
class SecureFS
{
public:
  SecureFS();

  void Rename(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);

private:

  void RenameFile(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);
  void RenameDirectory(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx);
};


}