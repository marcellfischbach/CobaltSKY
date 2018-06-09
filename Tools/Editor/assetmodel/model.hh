
#pragma once

#include <QObject>
#include <string>
#include <cobalt/core/csvfs.hh>


namespace asset::model
{
  class Asset;
  class Folder;
  class Root;
  class VFSEntry;
  class Model : public QObject
  {
    Q_OBJECT
  public:
    Model();
    virtual ~Model();

    Asset* CreateAsset(const std::string &fileName);
    Folder* CreateFolder(const std::string &folderName);
    VFSEntry* CreateVFSEntry(const csVFS::Entry &entry);

    Root *GetRoot() { return m_root;  }

  private:
    Root * m_root;

  };
}
