

#pragma once


#include <QObject>
#include <cobalt/core/csresourcelocator.hh>
#include <map>
#include <set>

class csfEntry;
namespace asset::model
{
	class Asset;
	class Entry;
	class Folder;
	class Model;
}

namespace asset::model::sync
{

	class RenameHandler : public QObject
	{
		Q_OBJECT;
	public:
		RenameHandler(asset::model::Model *model);

	private slots:
		void EntryAboutToRename(asset::model::Entry *entry);
		void EntryRenamed(asset::model::Entry *entry);

		void LocatorAboutToRename(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void LocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);



	private:
		void Rename(csfEntry *entry, const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		asset::model::Model * m_model;
		void RenameAsset(asset::model::Asset *asset, csResourceLocator locator);
		void RenameFolder(asset::model::Folder *folder, csResourceLocator locator);
		std::map<asset::model::Entry *, csResourceLocator> m_entries;
		std::map<csResourceLocator, std::set<asset::model::Entry*>> m_references;
	};
}