
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
		VFSEntry* CreateVFSEntry(const cs::VFS::Entry &entry);

    /**
     * @brief Checks whether the given locator is the locator with the lowest priority
     * @param locator The locator to check
     * @return \a True if the locator is the known locator with the lowest known priority. Else \a false.
     */
    bool IsRootLocator(const cs::ResourceLocator &locator) const;
    /**
     * @brief Checks whether the given locator is the locator with the hightes priority.
     * @param locator The locator to check
     * @return \a True if the locator is the known locator with the highest known priority. Else \a false.
     */
    bool IsFinalLocator(const cs::ResourceLocator &locator) const;

    const Entry *GetRootEntry(Entry* entry) const;
    const Entry *GetRootEntry(const cs::ResourceLocator &locator) const;
    Entry *GetRootEntry(Entry* entry);
    Entry *GetRootEntry(const cs::ResourceLocator &locator);

    const Entry *GetFinalEntry(Entry* entry) const;
    const Entry *GetFinalEntry(const cs::ResourceLocator &locator) const;
    Entry *GetFinalEntry(Entry* entry);
    Entry *GetFinalEntry(const cs::ResourceLocator &locator);

		Root *GetRoot() { return m_root; }
		Entry *GetEntry(const cs::ResourceLocator &locator);
		const Entry *GetEntry(const cs::ResourceLocator &locator) const;

		const std::set<Entry*> &GetAllEntriesFor(const cs::ResourceLocator &locator) const;
		std::set<Entry*> GetReferencing(const cs::ResourceLocator &locator) const;

    int GetRootPriority(const cs::ResourceLocator &locator) const;
    int GetFinalPriority(const cs::ResourceLocator &locator) const;

		void Add(Entry *parent, Entry *child, ModelTransaction &tr);
		void Remove(Entry *parent, Entry *child, ModelTransaction &tr);
		void Move(Entry *oldParent, Entry *newparent, Entry *child, ModelTransaction &tr);
		void Delete(Entry *entry, ModelTransaction &tr);
		void Rename(Entry *entry, const std::string &newname, ModelTransaction &tr);

		void ClearReferences(Entry *entry);
		void AddReference(Entry *entry, const cs::ResourceLocator &reference);

	signals:
		void EntryAdded(asset::model::Entry *parent, asset::model::Entry *child);
		void EntryRemoved(asset::model::Entry *parent, asset::model::Entry *child);
		void EntryRenamed(asset::model::Entry *entry, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
    void EntryDeleted(asset::model::Entry *entry, const cs::ResourceLocator &oldLocator);
		void ResourceRemoved(const cs::ResourceLocator &locator);
		void ResourceRenamed(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
    void ResourceChanged(const cs::ResourceLocator &locator);

	private:
		void AddCommit(Entry *parent, Entry *child);
		void AddRollback(Entry *parent, Entry *child);
    void RemoveCommit(Entry *parent, Entry *child, const cs::ResourceLocator &oldLocator);
		void RemoveRollback(Entry *parent, Entry *child);
		void MoveCommit(TreeCollector collector);
    void DeleteCommit(Entry *entry, const cs::ResourceLocator &locator);
		void RenameCommit(Entry *entry, const std::string &newName, TreeCollector collector);

		void MergePath(Entry *oldParent, Entry *newParent, Entry *child, ModelTransaction &tr);
		void UpdateCollector(TreeCollector &collector);
    void RenameCache(Entry *entry, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
		void InsertIntoEntryCache(Entry* entry);
		bool RemoveFromEntryCache(Entry* entry);
		void RemoveReference(const cs::ResourceLocator &locator);
		void RenameReference(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
		cs::ResourceLocator EvaluateEntryLocator(const Entry *entry) const;

	private:
		Root * m_root;

		std::map<cs::ResourceLocator, std::set<asset::model::Entry*>> m_entries;

		std::map<cs::ResourceLocator, asset::model::Entry*> m_namedEntries;

		std::map<asset::model::Entry *, std::set<cs::ResourceLocator>> m_references;
	};
}
