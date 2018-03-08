
#include <cobalt/loaders/csloaders.hh>
#include <cobalt/loaders/csblueprintcsfloader.hh>
#include <cobalt/loaders/csentitycsfloader.hh>
#include <cobalt/loaders/csentitystatecsfloader.hh>
#include <cobalt/loaders/cscollisionassetcsfloader.hh>
#include <cobalt/loaders/cspngimageassetloader.hh>
#include <cobalt/loaders/cspngimagefileloader.hh>
#include <cobalt/loaders/cspropertysettercsfloader.hh>
#include <cobalt/loaders/csmaterialassetcsfloader.hh>
#include <cobalt/loaders/csmaterialdefassetcsfloader.hh>
#include <cobalt/loaders/csmeshassetcsfloader.hh>
#include <cobalt/loaders/cssamplerassetcsfloader.hh>
#include <cobalt/loaders/csshadergraphassetcsfloader.hh>
#include <cobalt/loaders/csskinnedmeshassetcsfloader.hh>
#include <cobalt/loaders/csstaticmeshassetloader.hh>
#include <cobalt/loaders/csstaticmeshloader.hh>
#include <cobalt/loaders/cssubmeshassetloader.hh>
#include <cobalt/loaders/cstextureassetcsfloader.hh>



csLoaders::csLoaders()
{

}


void csLoaders::Register(csResourceManager *mgr)
{


  //
  // The new loaders go here
  mgr->RegisterLoader(new csBlueprintCSFLoader());

  mgr->RegisterLoader(new csCollisionAssetCSFLoader());
  mgr->RegisterLoader(new csEntityCSFLoader());
  mgr->RegisterLoader(new csEntityStateCSFLoader());
  mgr->RegisterLoader(new csMaterialAssetCSFLoader());
  mgr->RegisterLoader(new csMaterialDefAssetCSFLoader());
  mgr->RegisterLoader(new csMeshAssetCSFLoader());
  mgr->RegisterLoader(new csPropertySetterCSFLoader());
  mgr->RegisterLoader(new csSamplerAssetCSFLoader());
  mgr->RegisterLoader(new csShaderGraphAssetCSFLoader());
  mgr->RegisterLoader(new csSkinnedMeshAssetCSFLoader());
  mgr->RegisterLoader(new csTextureAssetCSFLoader());


  // image loaders
  mgr->RegisterLoader(new csPNGImageAssetLoader());
  mgr->RegisterLoader(new csPNGImageFileLoader());
  mgr->RegisterLoader(new csSubMeshAssetLoader());

}
