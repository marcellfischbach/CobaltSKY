

#include <Valkyrie/Assets/AssetManager.hh>


vkAssetManager::vkAssetManager()
{

}

vkAssetManager *vkAssetManager::Get()
{
  static vkAssetManager static_manager;
  return &static_manager;
}


