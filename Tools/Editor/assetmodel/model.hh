
#pragma once

#include <QObject>
#include <string>
#include <cobalt/core/csvfs.hh>


namespace asset::model
{
	namespace sync
	{
		class DeleteHandler;
	}
  class Asset;
	class Entry;
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

	signals:
		void EntryAboutToAdd(asset::model::Entry* parent, asset::model::Entry *child);
		void EntryAdded(asset::model::Entry *parent, asset::model::Entry *child);

		void EntryAboutToRemove(asset::model::Entry* parent, asset::model::Entry *child);
		void EntryRemove(asset::model::Entry *parent, asset::model::Entry *child);

		void EntryAboutToDelete(asset::model::Entry* entry);
		void EntryDeleted(asset::model::Entry* entry);

  private:
    Root * m_root;

		sync::DeleteHandler *m_deleteHandler;
  };
}
