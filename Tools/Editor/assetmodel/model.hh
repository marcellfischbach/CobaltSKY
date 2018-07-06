
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
	class ModelTransaction;
	class TreeCollector;
	class Model : public QObject
	{
		friend class AddCallback;
		friend class RemoveCallback;
		friend class RenameCallback;
		Q_OBJECT
	public:
		Model();
		virtual ~Model();

		Asset* CreateAsset(const std::string &fileName);
		Folder* CreateFolder(const std::string &folderName);
		VFSEntry* CreateVFSEntry(const csVFS::Entry &entry);

		Root *GetRoot() { return m_root; }
		Entry *GetEntry(const csResourceLocator &locator);
		const Entry *GetEntry(const csResourceLocator &locator) const;

		const std::set<Entry*> &GetAllEntriesFor(const csResourceLocator &locator) const;
		std::set<Entry*> GetReferencing(const csResourceLocator &locator) const;

		int GetLowestPriority(const csResourceLocator &locator) const;

		void Add(Entry *parent, Entry *child, ModelTransaction &tr);
		void Remove(Entry *parent, Entry *child, ModelTransaction &tr);
		void Delete(Entry *entry, ModelTransaction &tr);
		void Rename(Entry *entry, const std::string &newname, ModelTransaction &tr);

		void ClearReferences(Entry *entry);
		void AddReference(Entry *entry, const csResourceLocator &reference);

	signals:
		void EntryAdded(Entry *parent, Entry *child);
		void EntryRemoved(Entry *parent, Entry *child);
		void EntryRenamed(Entry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void ResourceRemoved(const csResourceLocator &locator);
		void ResourceRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);

	private:
		void AddCommit(Entry *parent, Entry *child);
		void AddRollback(Entry *parent, Entry *child);
		void RemoveCommit(Entry *parent, Entry *child);
		void RemoveRollback(Entry *parent, Entry *child);
		void RenameCommit(Entry *entry, const std::string &newName, TreeCollector &collector);

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
