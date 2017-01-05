
#include <Valkyrie/Loaders/vkloaders.hh>
#include <Valkyrie/Loaders/vkcollisionassetxmlloader.hh>
#include <Valkyrie/Loaders/vkpngimageassetloader.hh>
#include <Valkyrie/Loaders/vkpngimagefileloader.hh>
#include <Valkyrie/Loaders/vkmaterialassetxmlloader.hh>
#include <Valkyrie/Loaders/vkmaterialinstanceassetxmlloader.hh>
#include <Valkyrie/Loaders/vkmeshassetxmlloader.hh>
#include <Valkyrie/Loaders/vksamplerassetxmlloader.hh>
#include <Valkyrie/Loaders/vkshadergraphassetxmlloader.hh>
#include <Valkyrie/Loaders/vkstaticmeshassetloader.hh>
#include <Valkyrie/Loaders/vkstaticmeshloader.hh>
#include <Valkyrie/Loaders/vktextureassetxmlloader.hh>
#include <Valkyrie/Loaders/vkskinnedmeshassetxmlloader.hh>
#include <Valkyrie/Loaders/vksubmeshassetxmlloader.hh>


#include <Valkyrie/Loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkcolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkdynamiccolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentityloaderregistry.hh>
#include <Valkyrie/Loaders/entity/vkentitymasterxmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentitystatemasterxmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentitystatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkentityxmlloader.hh>
#include <Valkyrie/Loaders/entity/vkhingejointstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkjointstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkrenderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkspatialstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkstaticcolliderstatexmlloader.hh>
#include <Valkyrie/Loaders/entity/vkstaticmeshstatexmlloader.hh>

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
