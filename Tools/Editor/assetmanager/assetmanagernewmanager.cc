
#include <assetmanager/assetmanagernewmanager.hh>
#include <assetmanager/assetmanagernewhandler.hh>

AssetManagerNewManager::AssetManagerNewManager()
{
}

AssetManagerNewManager *AssetManagerNewManager::Get()
{
  static AssetManagerNewManager static_instance;
  return &static_instance;
}


void AssetManagerNewManager::RegisterNewHandler(AssetManagerNewHandler *handler)
{
  m_handlers.push_back(handler);
}

std::vector<AssetManagerNewHandler*> &AssetManagerNewManager::GetHandlers()
{
  return m_handlers;
}

const std::vector<AssetManagerNewHandler*> &AssetManagerNewManager::GetHandlers() const
{
  return m_handlers;
}
