
#include <cobalt/loaders/csloaders.hh>
#include <cobalt/loaders/cscollisionassetcsfloader.hh>
#include <cobalt/loaders/cscollisionassetxmlloader.hh>
#include <cobalt/loaders/cspngimageassetloader.hh>
#include <cobalt/loaders/cspngimagefileloader.hh>
#include <cobalt/loaders/csmaterialassetcsfloader.hh>
#include <cobalt/loaders/csmaterialassetxmlloader.hh>
#include <cobalt/loaders/csmaterialdefassetcsfloader.hh>
#include <cobalt/loaders/csmaterialdefassetxmlloader.hh>
#include <cobalt/loaders/csmeshassetcsfloader.hh>
#include <cobalt/loaders/csmeshassetxmlloader.hh>
#include <cobalt/loaders/cssamplerassetcsfloader.hh>
#include <cobalt/loaders/cssamplerassetxmlloader.hh>
#include <cobalt/loaders/csshadergraphassetcsfloader.hh>
#include <cobalt/loaders/csshadergraphassetxmlloader.hh>
#include <cobalt/loaders/csskinnedmeshassetcsfloader.hh>
#include <cobalt/loaders/csskinnedmeshassetxmlloader.hh>
#include <cobalt/loaders/csstaticmeshassetloader.hh>
#include <cobalt/loaders/csstaticmeshloader.hh>
#include <cobalt/loaders/cssubmeshassetloader.hh>
#include <cobalt/loaders/cstextureassetcsfloader.hh>
#include <cobalt/loaders/cstextureassetxmlloader.hh>


#include <cobalt/loaders/entity/csbasecolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/cscolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csdynamiccolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/loaders/entity/csentitymasterxmlloader.hh>
#include <cobalt/loaders/entity/csentitystatemasterxmlloader.hh>
#include <cobalt/loaders/entity/csentitystatexmlloader.hh>
#include <cobalt/loaders/entity/csentityxmlloader.hh>
#include <cobalt/loaders/entity/cshingejointstatexmlloader.hh>
#include <cobalt/loaders/entity/csjointstatexmlloader.hh>
#include <cobalt/loaders/entity/csrenderstatexmlloader.hh>
#include <cobalt/loaders/entity/csspatialstatexmlloader.hh>
#include <cobalt/loaders/entity/csstaticcolliderstatexmlloader.hh>
#include <cobalt/loaders/entity/csstaticmeshstatexmlloader.hh>

#include <cobalt/loaders/entity/csbasecolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/cscolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csdynamiccolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/loaders/entity/csentitymastercsfloader.hh>
#include <cobalt/loaders/entity/csentitystatemastercsfloader.hh>
#include <cobalt/loaders/entity/csentitystatecsfloader.hh>
#include <cobalt/loaders/entity/csentitycsfloader.hh>
#include <cobalt/loaders/entity/cshingejointstatecsfloader.hh>
#include <cobalt/loaders/entity/csjointstatecsfloader.hh>
#include <cobalt/loaders/entity/csrenderstatecsfloader.hh>
#include <cobalt/loaders/entity/csspatialstatecsfloader.hh>
#include <cobalt/loaders/entity/csstaticcolliderstatecsfloader.hh>
#include <cobalt/loaders/entity/csstaticmeshstatecsfloader.hh>

#include <cobalt/loaders/entity/blueprint/csblueprintcsfloader.hh>

csLoaders::csLoaders()
{

}


void csLoaders::Register(csResourceManager *mgr)
{


  //
  // The new loaders go here
  mgr->RegisterLoader(new csEntityMasterXMLLoader());
  mgr->RegisterLoader(new csEntityStateMasterXMLLoader());
  mgr->RegisterLoader(new csEntityMasterCSFLoader());
  mgr->RegisterLoader(new csEntityStateMasterCSFLoader());
  mgr->RegisterLoader(new csBlueprintCSFLoader());

  mgr->RegisterLoader(new csCollisionAssetCSFLoader());
  mgr->RegisterLoader(new csCollisionAssetXMLLoader());
  mgr->RegisterLoader(new csMaterialAssetCSFLoader());
  mgr->RegisterLoader(new csMaterialAssetXMLLoader());
  mgr->RegisterLoader(new csMaterialDefAssetCSFLoader());
  mgr->RegisterLoader(new csMaterialDefAssetXMLLoader());
  mgr->RegisterLoader(new csMeshAssetCSFLoader());
  mgr->RegisterLoader(new csMeshAssetXMLLoader());
  mgr->RegisterLoader(new csSamplerAssetCSFLoader());
  mgr->RegisterLoader(new csSamplerAssetXMLLoader());
  mgr->RegisterLoader(new csShaderGraphAssetCSFLoader());
  mgr->RegisterLoader(new csShaderGraphAssetXMLLoader());
  mgr->RegisterLoader(new csSkinnedMeshAssetCSFLoader());
  mgr->RegisterLoader(new csSkinnedMeshAssetXMLLoader());
  mgr->RegisterLoader(new csTextureAssetCSFLoader());
  mgr->RegisterLoader(new csTextureAssetXMLLoader());

  // image loaders
  mgr->RegisterLoader(new csPNGImageAssetLoader());
  mgr->RegisterLoader(new csPNGImageFileLoader());
  mgr->RegisterLoader(new csSubMeshAssetLoader());


  csEntityLoaderRegistry *reg = csEntityLoaderRegistry::Get();
  reg->RegisterLoader(new csEntityXMLLoader());
  reg->RegisterLoader(new csEntityStateXMLLoader());
  reg->RegisterLoader(new csSpatialStateXMLLoader());
  reg->RegisterLoader(new csRenderStateXMLLoader());
  reg->RegisterLoader(new csStaticMeshStateXMLLoader());
  reg->RegisterLoader(new csStaticColliderStateXMLLoader());
  reg->RegisterLoader(new csDynamicColliderStateXMLLoader());
  reg->RegisterLoader(new csJointStateXMLLoader());
  reg->RegisterLoader(new csHingeJointStateXMLLoader());


  reg->RegisterLoader(new csEntityCSFLoader());
  reg->RegisterLoader(new csEntityStateCSFLoader());
  reg->RegisterLoader(new csSpatialStateCSFLoader());
  reg->RegisterLoader(new csRenderStateCSFLoader());
  reg->RegisterLoader(new csStaticMeshStateCSFLoader());
  reg->RegisterLoader(new csStaticColliderStateCSFLoader());
  reg->RegisterLoader(new csDynamicColliderStateCSFLoader());
  reg->RegisterLoader(new csJointStateCSFLoader());
  reg->RegisterLoader(new csHingeJointStateCSFLoader());
}
