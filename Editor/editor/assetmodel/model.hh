
#pragma once

#include <editor/editorexport.hh>
#include <QObject>
#include <string>
#include <cobalt/core/csvfs.hh>
#include <editor/assetmodel/modelsync.hh>
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
	class ModelTransaction;
	class TreeCollector;
	class EDITOR_API Model : public QObject
	{
		Q_OBJECT
	public:
		Model();
		virtual ~Model();

		Asset* CreateAsset(const std::string &fileName);
		Folder* CreateFolder(const std::string &folderName);
		VFSEntry* CreateVFSEntry(const csVFS::Entry &entry);

    bool IsMasterEntry(Entry* entry) const;
    bool IsMasterLocator(const csResourceLocator &locator) const;

    const Entry *GetMasterEntry(Entry* entry) const;
    const Entry *GetMasterEntry(const csResourceLocator &locator) const;

    Entry *GetMasterEntry(Entry* entry);
    Entry *GetMasterEntry(const csResourceLocator &locator);

		Root *GetRoot() { return m_root; }
		Entry *GetEntry(const csResourceLocator &locator);
		const Entry *GetEntry(const csResourceLocator &locator) const;

		const std::set<Entry*> &GetAllEntriesFor(const csResourceLocator &locator) const;
		std::set<Entry*> GetReferencing(const csResourceLocator &locator) const;

		int GetLowestPriority(const csResourceLocator &locator) const;

		void Add(Entry *parent, Entry *child, ModelTransaction &tr);
		void Remove(Entry *parent, Entry *child, ModelTransaction &tr);
		void Move(Entry *oldParent, Entry *newparent, Entry *child, ModelTransaction &tr);
		void Delete(Entry *entry, ModelTransaction &tr);
		void Rename(Entry *entry, const std::string &newname, ModelTransaction &tr);

		void ClearReferences(Entry *entry);
		void AddReference(Entry *entry, const csResourceLocator &reference);

	signals:
		void EntryAdded(asset::model::Entry *parent, asset::model::Entry *child);
		void EntryRemoved(asset::model::Entry *parent, asset::model::Entry *child);
		void EntryRenamed(asset::model::Entry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
    void EntryDeleted(asset::model::Entry *entry, const csResourceLocator &oldLocator);
		void ResourceRemoved(const csResourceLocator &locator);
		void ResourceRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);

	private:
		void AddCommit(Entry *parent, Entry *child);
		void AddRollback(Entry *parent, Entry *child);
		void RemoveCommit(Entry *parent, Entry *child);
		void RemoveRollback(Entry *parent, Entry *child);
		void MoveCommit(TreeCollector collector);
    void DeleteCommit(Entry *entry, const csResourceLocator &locator);
		void RenameCommit(Entry *entry, const std::string &newName, TreeCollector collector);

		void MergePath(Entry *oldParent, Entry *newParent, Entry *child, ModelTransaction &tr);
		void UpdateCollector(TreeCollector &collector);
		void UpdateCache(Entry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void InsertIntoEntryCache(Entry* entry);
		bool RemoveFromEntryCache(Entry* entry);
		void RemoveReference(const csResourceLocator &locator);
		void RenameReference(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		csResourceLocator EvaluateEntryLocator(const Entry *entry) const;

	private:
		Root * m_root;

		std::map<csResourceLocator, std::set<asset::model::Entry*>> m_entries;

		std::map<csResourceLocator, asset::model::Entry*> m_namedEntries;

		std::map<asset::model::Entry *, std::set<csResourceLocator>> m_references;
	};
}
