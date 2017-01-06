
#include <Valkyrie/loaders/vkloaders.hh>
#include <Valkyrie/loaders/vkcollisionassetxmlloader.hh>
#include <Valkyrie/loaders/vkpngimageassetloader.hh>
#include <Valkyrie/loaders/vkpngimagefileloader.hh>
#include <Valkyrie/loaders/vkmaterialassetxmlloader.hh>
#include <Valkyrie/loaders/vkmaterialinstanceassetxmlloader.hh>
#include <Valkyrie/loaders/vkmeshassetxmlloader.hh>
#include <Valkyrie/loaders/vksamplerassetxmlloader.hh>
#include <Valkyrie/loaders/vkshadergraphassetxmlloader.hh>
#include <Valkyrie/loaders/vkstaticmeshassetloader.hh>
#include <Valkyrie/loaders/vkstaticmeshloader.hh>
#include <Valkyrie/loaders/vktextureassetxmlloader.hh>
#include <Valkyrie/loaders/vkskinnedmeshassetxmlloader.hh>
#include <Valkyrie/loaders/vksubmeshassetxmlloader.hh>


#include <Valkyrie/loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkcolliderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkdynamiccolliderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkentityloaderregistry.hh>
#include <Valkyrie/loaders/entity/vkentitymasterxmlloader.hh>
#include <Valkyrie/loaders/entity/vkentitystatemasterxmlloader.hh>
#include <Valkyrie/loaders/entity/vkentitystatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkentityxmlloader.hh>
#include <Valkyrie/loaders/entity/vkhingejointstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkjointstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkrenderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkspatialstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkstaticcolliderstatexmlloader.hh>
#include <Valkyrie/loaders/entity/vkstaticmeshstatexmlloader.hh>

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
