
#include <Valkyrie/Loaders/Loaders.hh>
#include <Valkyrie/Loaders/EntityLoader.hh>
#include <Valkyrie/Loaders/ImageLoader.hh>
#include <Valkyrie/Loaders/MaterialLoader.hh>
#include <Valkyrie/Loaders/MaterialInstanceLoader.hh>
#include <Valkyrie/Loaders/MeshLoader.hh>
#include <Valkyrie/Loaders/ShaderGraphLoader.hh>
#include <Valkyrie/Loaders/StaticMeshLoader.hh>
#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/ResourceManager.hh>


vkLoaders::vkLoaders()
{

}


void vkLoaders::Register(vkResourceManager *mgr)
{

  //
  // Legacy loaders

  mgr->RegisterLoader(new vkEntityMasterLoader());
  mgr->RegisterLoader(new vkEntityStateMasterLoader());
  //mgr->RegisterLoader(new vkMaterialLoader());
  //mgr->RegisterLoader(new vkMaterialInstanceLoader());
  mgr->RegisterLoader(new vkStaticMeshLoader());


  //
  // The new loaders go here
  mgr->RegisterLoader(new vkShaderGraphAssetXMLLoader());
  mgr->RegisterLoader(new vkMaterialInstanceAssetXMLLoader());
  mgr->RegisterLoader(new vkSamplerAssetXMLLoader());
  mgr->RegisterLoader(new vkTextureAssetXMLLoader());
  mgr->RegisterLoader(new vkMeshAssetXMLLoader());

  // image loaders
  mgr->RegisterLoader(new vkPNGImageAssetLoader());
  mgr->RegisterLoader(new vkPNGImageFileLoader());
  mgr->RegisterLoader(new vkSubMeshAssetLoader());


  vkEntityLoaderRegistry *reg = vkEntityLoaderRegistry::Get();
  reg->RegisterLoader(new vkEntityLoader());
  reg->RegisterLoader(new vkEntityStateLoader());
  reg->RegisterLoader(new vkSpatialStateLoader());
  reg->RegisterLoader(new vkRenderStateLoader());
  reg->RegisterLoader(new vkStaticMeshStateLoader());
  reg->RegisterLoader(new vkStaticColliderStateLoader());
  reg->RegisterLoader(new vkDynamicColliderStateLoader());
  reg->RegisterLoader(new vkJointStateLoader());
  reg->RegisterLoader(new vkHingeJointStateLoader());
}
