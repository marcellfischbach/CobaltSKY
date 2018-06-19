
#include <assetmodel/sync/deletehandler.hh>
#include <assetmodel/sync/assetmodifier.hh>
#include <assetmodel/sync/assetscanner.hh>
#include <assetmodel/model.hh>
#include <assetmodel/entry.hh>
#include <filesystem>


namespace asset::model::sync
{

  DeleteHandler::DeleteHandler(asset::model::Model *model)
    :QObject()
    , m_model(model)
  {
    connect(model, SIGNAL(LocatorAboutToRemove(const csResourceLocator&)), this, SLOT(LocatorAboutToRemove(const csResourceLocator &)));
    connect(model, SIGNAL(LocatorRemoved(const csResourceLocator&)), this, SLOT(LocatorRemoved(const csResourceLocator &)));
  }


  void DeleteHandler::LocatorAboutToRemove(const csResourceLocator &locator)
  {
    csResourceLocator anonLocator = locator.AsAnonymous();
    std::set<Entry*> entries = m_model->GetReferencing(anonLocator);
    m_entries[anonLocator] = entries;
  }

  void DeleteHandler::LocatorRemoved(const csResourceLocator &locator)
  {
    auto it = m_entries.find(locator.AsAnonymous());
    if (it == m_entries.end())
    {
      return;
    }
    csResourceLocator anonLocator = locator.AsAnonymous();
    for (Entry *entry : it->second)
    {
      Asset *asset = entry->AsAsset();
      if (asset)
      {
        AssetModifier(asset).ClearLocator(anonLocator);
        AssetScanner(asset).Scan();
      }
    }
    m_entries.erase(it);

  }
}