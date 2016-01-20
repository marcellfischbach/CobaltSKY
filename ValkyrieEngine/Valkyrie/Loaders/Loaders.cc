
#include <Valkyrie/Loaders/Loaders.hh>
#include <Valkyrie/Loaders/EntityLoader.hh>
#include <Valkyrie/Loaders/ImageLoader.hh>
#include <Valkyrie/Loaders/MaterialLoader.hh>
#include <Valkyrie/Loaders/StaticMeshLoader.hh>
#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/ResourceManager.hh>


vkLoaders::vkLoaders()
{

}


void vkLoaders::Register(vkResourceManager *mgr)
{
  mgr->RegisterLoader(new vkEntityMasterLoader());
  mgr->RegisterLoader(new vkEntityStateMasterLoader());
  mgr->RegisterLoader(new vkMaterialLoader());
  mgr->RegisterLoader(new vkMaterialInstanceLoader());
  mgr->RegisterLoader(new vkPNGImageLoader());
  mgr->RegisterLoader(new vkSamplerLoader());
  mgr->RegisterLoader(new vkStaticMeshLoader());
  mgr->RegisterLoader(new vkTextureLoader());

  vkEntityLoaderRegistry *reg = vkEntityLoaderRegistry::Get();
  reg->RegisterLoader(new vkEntityLoader());
  reg->RegisterLoader(new vkEntityStateLoader());
  reg->RegisterLoader(new vkSpatialStateLoader());
  reg->RegisterLoader(new vkRenderStateLoader());
  reg->RegisterLoader(new vkStaticMeshStateLoader());
  reg->RegisterLoader(new vkRigidBodyStateLoader());
  reg->RegisterLoader(new vkStaticColliderStateLoader());
  reg->RegisterLoader(new vkDynamicColliderStateLoader());
}
