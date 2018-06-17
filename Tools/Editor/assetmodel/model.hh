
#pragma once

#include <QObject>
#include <string>
#include <cobalt/core/csvfs.hh>
#include <map>
#include <set>



namespace asset::model
{
	namespace sync
	{
		class DeleteHandler;
		class RenameHandler;
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

    const std::set<Entry*> GetEntries(const csResourceLocator &locator) const;
		const std::set<Entry*> GetReferencing(const csResourceLocator &locator) const;

    void RemoveReferences(Entry *entry);
		void AddReference(Entry *entry, const csResourceLocator &locator);

	signals:
		void EntryAboutToAdd(asset::model::Entry* parent, asset::model::Entry *child);
		void EntryAdded(asset::model::Entry *parent, asset::model::Entry *child);

		void EntryAboutToRemove(asset::model::Entry* parent, asset::model::Entry *child);
		void EntryRemoved(asset::model::Entry *parent, asset::model::Entry *child);

		void EntryAboutToRename(asset::model::Entry *entry);
		void EntryRenamed(asset::model::Entry *entry);

		void EntryAboutToMove(asset::model::Entry *entry);
		void EntryMoved(asset::model::Entry *entry);

		void EntryAboutToDelete(asset::model::Entry* entry);
		void EntryDeleted(asset::model::Entry* entry);

		void LocatorAboutToRename(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void LocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);

  private slots:
    void onEntryAdded(asset::model::Entry *parent, asset::model::Entry *child);
    void onEntryRemoved(asset::model::Entry *parent, asset::model::Entry *child);
    void onEntryRenamed(asset::model::Entry *entry);
		void onLocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);


  private:
		
		csResourceLocator GetCurrentResourceLocator(Entry*entry) const;
		int GetLowestPriority(const csResourceLocator &locator) const;
    Root * m_root;

		sync::DeleteHandler *m_deleteHandler;
		sync::RenameHandler *m_renameHandler;

    std::map<csResourceLocator, std::set<Entry*>> m_entries;
		std::map<Entry*, std::set<csResourceLocator>> m_references;
  };
}
