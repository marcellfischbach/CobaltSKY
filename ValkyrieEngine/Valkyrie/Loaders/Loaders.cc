
#include <Valkyrie/Loaders/Loaders.hh>
#include <Valkyrie/Loaders/ImageLoader.hh>
#include <Valkyrie/Loaders/MaterialLoader.hh>
#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/ResourceManager.hh>


vkLoaders::vkLoaders()
{

}


void vkLoaders::Register(vkResourceManager *mgr)
{
  mgr->RegisterLoader(new vkMaterialLoader());
  mgr->RegisterLoader(new vkMaterialInstanceLoader());
  mgr->RegisterLoader(new vkPNGImageLoader());
  mgr->RegisterLoader(new vkTextureLoader());
  mgr->RegisterLoader(new vkSamplerLoader());
}
