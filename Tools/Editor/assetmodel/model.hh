
#pragma once

#include <QObject>
#include <string>
#include <cobalt/core/csvfs.hh>
#include <assetmodel/modelsync.hh>
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
	class MoveHelper;
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

		int GetLowestPriority(const csResourceLocator &locator) const;

		/**
		 * Not part of the public API
		 * @{
		*/
		void Add(Entry *parent, Entry *child);
		void Remove(Entry *parent, Entry *child);
		void Rename(Entry *entry, const std::string &name);
		void Delete(Entry *entry);

		/**
		 * @}
		 */

	signals:
		void EntryAboutToAdd(asset::model::Entry* parent, asset::model::Entry *child);
		void EntryAdded(asset::model::Entry *parent, asset::model::Entry *child);

		void EntryAboutToRemove(asset::model::Entry* parent, asset::model::Entry *child);
		void EntryRemoved(asset::model::Entry *parent, asset::model::Entry *child);

		void EntryAboutToChanged(asset::model::Entry* entry);
		void EntryChanged(asset::model::Entry* entry);

		void LocatorAboutToRename(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void LocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void LocatorAboutToRemove(const csResourceLocator &locator);
		void LocatorRemoved(const csResourceLocator &locator);

  private slots:


  private:
    void AddEntry(Entry *entry);
    bool IsLastEntry(Entry *entry);
    bool RemoveEntry(Entry *entry);

		void HandleLocatorRenamed(MoveHelper &helper);
    void HandleLocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
    void HandleLocatorRemoved(const csResourceLocator &locator);

		csResourceLocator GetCurrentResourceLocator(Entry*entry) const;
    Root * m_root;

		sync::DeleteHandler *m_deleteHandler;
		sync::RenameHandler *m_renameHandler;

    std::map<csResourceLocator, std::set<Entry*>> m_entries;
		std::map<Entry*, std::set<csResourceLocator>> m_references;

		ModelSync m_sync;
  };
}
