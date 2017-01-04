
#include <Valkyrie/Loaders/Loaders.hh>
#include <Valkyrie/Loaders/CollisionLoader.hh>
#include <Valkyrie/Loaders/EntityLoader.hh>
#include <Valkyrie/Loaders/ImageLoader.hh>
#include <Valkyrie/Loaders/MaterialLoader.hh>
#include <Valkyrie/Loaders/MaterialInstanceLoader.hh>
#include <Valkyrie/Loaders/MeshLoader.hh>
#include <Valkyrie/Loaders/ShaderGraphLoader.hh>
#include <Valkyrie/Loaders/StaticMeshLoader.hh>
#include <Valkyrie/Loaders/TextureLoader.hh>
#include <Valkyrie/Core/vkresourcemanager.hh>


vkLoaders::vkLoaders()
{

}


void vkLoaders::Register(vkResourceManager *mgr)
{


  //
  // The new loaders go here
  mgr->RegisterLoader(new vkEntityMasterXMLLoader());
  mgr->RegisterLoader(new vkEntityStateMasterXMLLoader());

  mgr->RegisterLoader(new vkShaderGraphAssetXMLLoader());
  mgr->RegisterLoader(new vkMaterialAssetXMLLoader());
  mgr->RegisterLoader(new vkMaterialInstanceAssetXMLLoader());
  mgr->RegisterLoader(new vkSamplerAssetXMLLoader());
  mgr->RegisterLoader(new vkTextureAssetXMLLoader());
  mgr->RegisterLoader(new vkMeshAssetXMLLoader());
  mgr->RegisterLoader(new vkSkinnedMeshAssetXMLLoader());
  mgr->RegisterLoader(new vkCollisionAssetXMLLoader());

  // image loaders
  mgr->RegisterLoader(new vkPNGImageAssetLoader());
  mgr->RegisterLoader(new vkPNGImageFileLoader());
  mgr->RegisterLoader(new vkSubMeshAssetLoader());


  vkEntityLoaderRegistry *reg = vkEntityLoaderRegistry::Get();
  reg->RegisterLoader(new vkEntityXMLLoader());
  reg->RegisterLoader(new vkEntityStateXMLLoader());
  reg->RegisterLoader(new vkSpatialStateXMLLoader());
  reg->RegisterLoader(new vkRenderStateXMLLoader());
  reg->RegisterLoader(new vkStaticMeshStateXMLLoader());
  reg->RegisterLoader(new vkStaticColliderStateXMLLoader());
  reg->RegisterLoader(new vkDynamicColliderStateXMLLoader());
  reg->RegisterLoader(new vkJointStateXMLLoader());
  reg->RegisterLoader(new vkHingeJointStateXMLLoader());
}
