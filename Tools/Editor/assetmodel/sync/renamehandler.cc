
#include <assetmodel/sync/renamehandler.hh>
#include <assetmodel/sync/assetmodifier.hh>
#include <assetmodel/sync/assetscanner.hh>
#include <assetmodel/asset.hh>
#include <assetmodel/folder.hh>
#include <assetmodel/model.hh>
#include <assetmodel/entry.hh>
#include <filesystem>


namespace asset::model::sync
{

	RenameHandler::RenameHandler(asset::model::Model *model)
		:QObject()
		, m_model(model)
	{
		connect(model, SIGNAL(LocatorAboutToRename(const csResourceLocator &, const csResourceLocator &)), this, SLOT(LocatorAboutToRename(const csResourceLocator &, const csResourceLocator &)));
		connect(model, SIGNAL(LocatorRenamed(const csResourceLocator &, const csResourceLocator &)), this, SLOT(LocatorRenamed(const csResourceLocator &, const csResourceLocator &)));
	}

  void RenameHandler::LocatorAboutToRename(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
  {
    csResourceLocator anonLocator = oldLocator.AsAnonymous();
    std::set<Entry*> entries = m_model->GetReferencing(anonLocator);
    m_entries[anonLocator] = entries;

  }
	void RenameHandler::LocatorRenamed(const csResourceLocator &oldLocator, const csResourceLocator &newLocator)
	{
    csResourceLocator anonOldLocator = oldLocator.AsAnonymous();
    auto it = m_entries.find(anonOldLocator);
    if (it == m_entries.end())
    {
      return;
    }
    csResourceLocator anonNewLocator = newLocator.AsAnonymous();
		for (Entry *entry : it->second)
		{
      Asset *asset = entry->AsAsset();
      if (asset)
      {
        AssetModifier(asset).ReplaceLocator(anonOldLocator, anonNewLocator);
        AssetScanner(asset).Scan();
      }
		}
    m_entries.erase(it);
	}


}