

#include <editormodel/fs/secfs.hh>
#include <editormodel/modelexceptions.hh>
#include <editormodel/transaction.hh>
#include <chrono>
#include <ctime>

namespace cs::editor::model
{


SecureFS::SecureFS()
{

}

/* ************************************************************************
 * ************************************************************************
 *         Rename
 * ************************************************************************
 * ************************************************************************/


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




/* ************************************************************************
 * ************************************************************************
 *         Delete
 * ************************************************************************
 * ************************************************************************/


void SecureFS::Delete(const std::filesystem::path &path, bool forceDelete, Transaction &tx)
{
  if (!std::filesystem::exists(path))
  {
    printf("Unable to delete %s. The file or directory does not exist.", path.c_str());
    return;
  }

  if (std::filesystem::is_regular_file(path))
  {
    DeleteFile(path, forceDelete, tx);
  }
  else if (std::filesystem::is_directory(path))
  {
    DeleteDirectory(path, forceDelete, tx);
  }

}


void SecureFS::DeleteFile(const std::filesystem::path &path, bool forceDelete, Transaction &tx)
{
  ValidateTempDirectory();

  std::filesystem::path tmp = GenerateUniqueTempPath();
  std::filesystem::copy(path, tmp);
  std::filesystem::remove(path);
  tx.OnCommit([tmp]() {
    std::filesystem::remove(tmp);
  });
  tx.OnRollback([path, tmp]() {
    std::filesystem::copy(tmp, path);
    std::filesystem::remove(tmp);
  });

}

void SecureFS::DeleteDirectory(const std::filesystem::path &path, bool forceDelete, Transaction &tx)
{
  if (!std::filesystem::is_empty(path) && !forceDelete)
  {
    throw FSDirectoryNotEmptyException("The given path cannot be deleted. It is not empty", path);
  }

  if (forceDelete)
  {
    std::filesystem::remove_all(path);
  }
  else
  {
    std::filesystem::remove(path);
  }
  tx.OnRollback([path]() {
    std::filesystem::create_directories(path);
  });

}


void SecureFS::ValidateTempDirectory()
{
  std::filesystem::path tmp = std::filesystem::temp_directory_path();
  std::filesystem::path editorTemp = tmp / "CobaltSKY-Editor";
  std::filesystem::create_directories(editorTemp);
}

std::filesystem::path SecureFS::GenerateUniqueTempPath()
{
  static unsigned long c = 0;
  c++;

  auto now = std::chrono::system_clock::now();

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

  std::string name = "_" + std::to_string(ms.count()) + "_" + std::to_string(c);

  std::filesystem::path tmp = std::filesystem::temp_directory_path();
  std::filesystem::path editorTemp = tmp / "CobaltSKY-Editor";

  return editorTemp / name;
}



}