
#pragma once

#include <QObject>
#include <cobalt/core/csresourcelocator.hh>

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

	class DeleteHandler : public QObject
	{
		Q_OBJECT;
	public:
		DeleteHandler(asset::model::Model *model);

	private slots:
		void EntryAboutToDelete(asset::model::Entry *entry);
		void EntryDeleted(asset::model::Entry *entry);

	private:
    asset::model::Model *m_model;

    void ClearAssetReference(const csResourceLocator &locator);
    void ClearAssetReference(csfEntry *entry, const csResourceLocator &locator);
		void DeleteAsset(asset::model::Asset *asset, csResourceLocator locator);
		void DeleteFolder(asset::model::Folder *folder, csResourceLocator locator);
		std::map<asset::model::Entry *, csResourceLocator> m_entries;
	};
}

