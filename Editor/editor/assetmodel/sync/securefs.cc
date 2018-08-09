
#include <editor/assetmodel/sync/securefs.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
#include <QDir>
#include <QUuid>

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

  class SecureFSMoveFile : public ModelTransaction::iCallback
  {
  public:
    SecureFSMoveFile(std::filesystem::path &oldPath, std::filesystem::path &newPath)
      : ModelTransaction::iCallback()
      , m_oldPath(oldPath)
      , m_newPath(newPath)
    {

    }

    virtual ~SecureFSMoveFile()
    {

    }

    void OnCommit()
    {
      try
      {
        std::filesystem::remove(m_oldPath);
      }
      catch (const std::exception &e)
      {

      }
    }
    void OnRollback()
    {
      try
      {
        std::filesystem::remove(m_newPath);
      }
      catch (const std::exception &e)
      {

      }
    }

  private:
    std::filesystem::path m_oldPath;
    std::filesystem::path m_newPath;
  };


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

    m_tr.Attach(new SecureFSMoveFile(oldPath, newPath));
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


  void SecureFS::Delete(const csResourceLocator &locator)
  {
    std::filesystem::path path(csVFS::Get()->GetAbsolutePath(locator, csVFS::CheckExistence));

    Delete(path);
  }


  void SecureFS::Delete(std::filesystem::path &path)
  {
    if (!std::filesystem::exists(path))
    {
      throw AlterFSException("Path " + path.generic_string() + " does not exsist");
    }

    if (std::filesystem::is_regular_file(path))
    {
      DeleteFile(path);
    }
    else if (std::filesystem::is_directory(path))
    {
      DeleteDirectory(path);
    }
  }

  class SecureFSDeleteFile : public ModelTransaction::iCallback
  {
  public:
    SecureFSDeleteFile(std::filesystem::path &file, std::filesystem::path &temp)
      : m_file(file)
      , m_temp(temp)
    {
    }
    virtual ~SecureFSDeleteFile()
    {

    }

    void OnCommit()
    {
      try
      {
        std::filesystem::remove(m_temp);
      }
      catch (const std::exception &e)
      {

      }
    }

    void OnRollback()
    {
      try
      {
        std::filesystem::copy(m_temp, m_file);
      }
      catch (const std::exception &e)
      {

      }
      try
      {
        std::filesystem::remove(m_temp);
      }
      catch (const std::exception &e)
      {

      }
    }

  private:
    std::filesystem::path m_file;
    std::filesystem::path m_temp;
  };

  void SecureFS::DeleteFile(std::filesystem::path &path)
  {
    if (!std::filesystem::exists(path))
    {
      throw AlterFSException("Path " + path.generic_string() + " does not exsist");
    }

    if (!std::filesystem::is_regular_file(path))
    {
      throw AlterFSException("Path " + path.generic_string() + " is no file");
    }

    std::filesystem::path tmpFile = GetTempFile();
    bool saveDelete = true;
    try
    {
      std::filesystem::copy_file(path, tmpFile);
    }
    catch (const std::exception &e)
    {
      saveDelete = false;
    }

    try
    {
      if (!std::filesystem::remove(path))
      {
        throw AlterFSException("Unable to delete " + path.generic_string());
      }
    }
    catch (const std::exception &e)
    {
      throw AlterFSException("Unable to delete " + path.generic_string());
    }

    if (saveDelete)
    {
      m_tr.Attach(new SecureFSDeleteFile(path, tmpFile));
    }
  }


  class SecureFSDeleteDirectory : public ModelTransaction::iCallback
  {
  public:
    SecureFSDeleteDirectory(std::filesystem::path &dir)
      : m_dir(dir)
    {
    }
    virtual ~SecureFSDeleteDirectory()
    {

    }

    void OnCommit()
    {
    }

    void OnRollback()
    {
      try
      {
        std::filesystem::create_directories(m_dir);
      }
      catch (const std::exception &e)
      {

      }
    }

  private:
    std::filesystem::path m_dir;
  };

  void SecureFS::DeleteDirectory(std::filesystem::path &path)
  {
    if (!std::filesystem::exists(path))
    {
      throw AlterFSException("Path " + path.generic_string() + " does not exsist");
    }

    if (!std::filesystem::is_directory(path))
    {
      throw AlterFSException("Path " + path.generic_string() + " is no directory");
    }


    for (auto &e : std::filesystem::directory_iterator(path))
    {
      std::filesystem::path childPath = e.path();
      Delete(childPath);
    }

    try
    {
      if (!std::filesystem::remove(path))
      {
        throw AlterFSException("Unable to delete " + path.generic_string());
      }
    }
    catch (const std::exception &e)
    {
      throw AlterFSException("Unable to delete " + path.generic_string());
    }
    m_tr.Attach(new SecureFSDeleteDirectory(path));
  }


  std::filesystem::path SecureFS::GetTempFile()
  {
    std::string randomName = std::string(QUuid::createUuid().toString().toLatin1().data());
    std::string homePath = QDir::homePath().toLatin1().data();
    std::string editorPath = homePath + "/CobalSKY-Editor";

    std::filesystem::path path(editorPath);
    if (!std::filesystem::exists(path))
    {
      if (!std::filesystem::create_directories(std::filesystem::path(editorPath)))
      {
        throw AlterFSException("Unable to create editor path: " + editorPath);
      }
    }

    std::string tempPath = editorPath + "/" + randomName;

    return std::filesystem::path(tempPath);
  }
}
