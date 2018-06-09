
#include <assetmodel/model.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>

namespace asset::model
{
  Model::Model()
    : QObject()
  {
    m_root = new Root(this);
  }

  Model::~Model()
  {

  }


  Asset *Model::CreateAsset(const std::string &fileName)
  {
    return new Asset(this, fileName);
  }

  Folder *Model::CreateFolder(const std::string &folderName)
  {
    return new Folder(this, folderName);
  }

  VFSEntry *Model::CreateVFSEntry(const csVFS::Entry &entry)
  {
    return new VFSEntry(this, entry);
  }

}
