
#include <valkyrie/loaders/vkloaders.hh>
#include <valkyrie/loaders/vkcollisionassetxmlloader.hh>
#include <valkyrie/loaders/vkpngimageassetloader.hh>
#include <valkyrie/loaders/vkpngimagefileloader.hh>
#include <valkyrie/loaders/vkmaterialassetxmlloader.hh>
#include <valkyrie/loaders/vkmaterialinstanceassetxmlloader.hh>
#include <valkyrie/loaders/vkmeshassetxmlloader.hh>
#include <valkyrie/loaders/vksamplerassetxmlloader.hh>
#include <valkyrie/loaders/vkshadergraphassetxmlloader.hh>
#include <valkyrie/loaders/vkstaticmeshassetloader.hh>
#include <valkyrie/loaders/vkstaticmeshloader.hh>
#include <valkyrie/loaders/vktextureassetxmlloader.hh>
#include <valkyrie/loaders/vkskinnedmeshassetxmlloader.hh>
#include <valkyrie/loaders/vksubmeshassetxmlloader.hh>


#include <valkyrie/loaders/entity/vkbasecolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkcolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkdynamiccolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentityloaderregistry.hh>
#include <valkyrie/loaders/entity/vkentitymasterxmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystatemasterxmlloader.hh>
#include <valkyrie/loaders/entity/vkentitystatexmlloader.hh>
#include <valkyrie/loaders/entity/vkentityxmlloader.hh>
#include <valkyrie/loaders/entity/vkhingejointstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkjointstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkrenderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkspatialstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkstaticcolliderstatexmlloader.hh>
#include <valkyrie/loaders/entity/vkstaticmeshstatexmlloader.hh>

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
