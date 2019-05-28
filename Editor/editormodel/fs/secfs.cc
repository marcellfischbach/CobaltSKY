

#include <editormodel/fs/secfs.hh>
#include <editormodel/modelexceptions.hh>
#include <editormodel/transaction.hh>
#include <chrono>
#include <ctime>
#include <iostream>

namespace cs::editor::model
{


SecureFS::SecureFS()
{

}


/* ************************************************************************
 * ************************************************************************
 *         Create folder
 * ************************************************************************
 * ************************************************************************/

void SecureFS::CreateFolder(const std::filesystem::path &path, Transaction &tx)
{
  if (std::filesystem::exists(path))
  {
    return;
  }

  std::filesystem::create_directory(path);
  tx.OnRollback([this, path]() { std::filesystem::remove(path); });
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
    std::cerr << "Unable to rename " << sourcePath.c_str() << ".The file or directory does not exist.\n";
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


void SecureFS::Delete(const std::filesystem::path &path, Transaction &tx)
{
  if (!std::filesystem::exists(path))
  {
    std::cerr << "Unable to delete " << path.c_str() << ". The file or directory does not exist.\n";
    return;
  }

  if (std::filesystem::is_regular_file(path))
  {
    DeleteFile(path, tx);
  }
  else if (std::filesystem::is_directory(path))
  {
    DeleteDirectory(path, tx);
  }

}


void SecureFS::DeleteFile(const std::filesystem::path &path, Transaction &tx)
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

void SecureFS::DeleteDirectory(const std::filesystem::path &path, Transaction &tx)
{
  if (!std::filesystem::is_empty(path))
  {
    throw FSDirectoryNotEmptyException("The given path cannot be deleted. It is not empty", path);
  }


  std::filesystem::remove(path);

  tx.OnRollback([path]() {
    std::filesystem::create_directories(path);
  });

}




/* ************************************************************************
 * ************************************************************************
 *         Move
 * ************************************************************************
 * ************************************************************************/


void SecureFS::Move(const std::filesystem::path &sourcePath, const std::filesystem::path &destinationPath, Transaction &tx)
{
  if (!std::filesystem::is_directory(destinationPath))
  {
    throw FSMoveTargetNotADirectory("The target of a move operation is not a directory", destinationPath);
  }

  if (std::filesystem::is_regular_file(sourcePath))
  {
    MoveFile(sourcePath, destinationPath, tx);
  }

}

void SecureFS::MoveFile(const std::filesystem::path &sourceFilePath, const std::filesystem::path &folderPath, Transaction &tx)
{
  std::filesystem::path destinationFilePath = folderPath / sourceFilePath.filename();
  if (std::filesystem::exists(destinationFilePath))
  {
    throw FSMoveTargetAlreadyExisting("Destination filename already existing", destinationFilePath);
  }

  std::filesystem::rename(sourceFilePath, destinationFilePath);

  tx.OnRollback([sourceFilePath, destinationFilePath]() {
    std::filesystem::rename(destinationFilePath, sourceFilePath);
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