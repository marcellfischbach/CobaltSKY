

#include <editormodel/fs/secfs.hh>
#include <editormodel/transaction.hh>

namespace cs::editor::model
{


SecureFS::SecureFS()
{

}


void SecureFS::Rename(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx)
{
  if (!std::filesystem::exists(sourcePath))
  {
    printf("Unable to rename %s. The file or directory does not exist.", sourcePath.c_str());
    return;
  }
  if (std::filesystem::exists(destinationPath))
  {
    throw FSModificationException("Unable to rename. Destination " + destinationPath.generic_string() + " already exists.");
  }

  if (std::filesystem::is_regular_file(sourcePath))
  {
    RenameFile(sourcePath, destinationPath, tx);
  }
  else if (std::filesystem::is_directory(sourcePath))
  {
    RenameDirectory(sourcePath, destinationPath, tx);
  }
}


void SecureFS::RenameFile(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx) 
{
  std::filesystem::rename(sourcePath, destinationPath);
  tx.OnRollback([sourcePath, destinationPath]() { std::filesystem::rename(destinationPath, sourcePath); });
}



void SecureFS::RenameDirectory(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx)
{
  std::filesystem::rename(sourcePath, destinationPath);
  tx.OnCommit([sourcePath]() { std::filesystem::remove_all(sourcePath); });
  tx.OnRollback([destinationPath]() { std::filesystem::remove_all(destinationPath); });
}

}