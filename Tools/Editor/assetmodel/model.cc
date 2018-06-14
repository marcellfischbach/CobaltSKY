
#include <assetmodel/model.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/root.hh>
#include <assetmodel/vfsentry.hh>
#include <assetmodel/sync/deletehandler.hh>

namespace asset::model
{
  Model::Model()
    : QObject()
  {
    m_root = new Root(this);
		m_deleteHandler = new sync::DeleteHandler(this);

    connect(this, SIGNAL(EntryAdded(asset::model::Entry*, asset::model::Entry*)), SLOT(onEntryAdded(asset::model::Entry*, asset::model::Entry*)));
    connect(this, SIGNAL(EntryRemoved(asset::model::Entry*, asset::model::Entry*)), SLOT(onEntryRemoved(asset::model::Entry*, asset::model::Entry*)));
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

  static std::set<Entry*> EmptyEntrySet;

  const std::set<Entry*> &Model::GetEntries(const csResourceLocator &locator) const
  {
    csResourceLocator anonLocator = locator.AsAnonymous();
    auto it = m_entries.find(anonLocator);
    if (it == m_entries.end())
    {
      return EmptyEntrySet;
    }
    return it->second;
  }

  void Model::onEntryAdded(Entry *parent, Entry *child)
  {
    csResourceLocator locator = child->GetResourceLocator();
    csResourceLocator anonLocator = locator.AsAnonymous();
    m_entries[anonLocator].insert(child);
  }

  void Model::onEntryRemoved(Entry *parent, Entry *child)
  {
    for (auto it = m_entries.begin(); it != m_entries.end(); ++it)
    {
      auto mit = it->second.find(child);
      if (mit != it->second.end())
      {
        it->second.erase(mit);
        if (it->second.empty())
        {
          m_entries.erase(it);
        }
        return;
      }
    }
  }



}
