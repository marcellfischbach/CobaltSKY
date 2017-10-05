
#include <cobalt/loaders/csloaders.hh>
#include <cobalt/loaders/cscollisionassetxmlloader.hh>
#include <cobalt/loaders/cspngimageassetloader.hh>
#include <cobalt/loaders/cspngimagefileloader.hh>
#include <cobalt/loaders/csmaterialassetxmlloader.hh>
#include <cobalt/loaders/csmaterialdefassetxmlloader.hh>
#include <cobalt/loaders/csmeshassetxmlloader.hh>
#include <cobalt/loaders/cssamplerassetcsfloader.hh>
#include <cobalt/loaders/cssamplerassetxmlloader.hh>
#include <cobalt/loaders/csshadergraphassetxmlloader.hh>
#include <cobalt/loaders/csstaticmeshassetloader.hh>
#include <cobalt/loaders/csstaticmeshloader.hh>
#include <cobalt/loaders/cstextureassetxmlloader.hh>
#include <cobalt/loaders/csskinnedmeshassetxmlloader.hh>
#include <cobalt/loaders/cssubmeshassetxmlloader.hh>


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

csLoaders::csLoaders()
{

}


void csLoaders::Register(csResourceManager *mgr)
{


  //
  // The new loaders go here
  mgr->RegisterLoader(new csEntityMasterXMLLoader());
  mgr->RegisterLoader(new csEntityStateMasterXMLLoader());

  mgr->RegisterLoader(new csShaderGraphAssetXMLLoader());
  mgr->RegisterLoader(new csMaterialAssetXMLLoader());
  mgr->RegisterLoader(new csMaterialDefAssetXMLLoader());
  mgr->RegisterLoader(new csSamplerAssetXMLLoader());
  mgr->RegisterLoader(new csSamplerAssetCSFLoader());
  mgr->RegisterLoader(new csTextureAssetXMLLoader());
  mgr->RegisterLoader(new csMeshAssetXMLLoader());
  mgr->RegisterLoader(new csSkinnedMeshAssetXMLLoader());
  mgr->RegisterLoader(new csCollisionAssetXMLLoader());

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
}
