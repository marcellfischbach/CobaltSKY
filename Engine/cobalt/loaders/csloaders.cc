
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



cs::Loaders::Loaders()
{

}


void cs::Loaders::Register(cs::ResourceManager *mgr)
{


  //
  // The new loaders go here
  mgr->RegisterLoader(new cs::BlueprintCSFLoader());

  mgr->RegisterLoader(new cs::CollisionAssetCSFLoader());
  mgr->RegisterLoader(new cs::EntityCSFLoader());
  mgr->RegisterLoader(new cs::EntityStateCSFLoader());
  mgr->RegisterLoader(new cs::MaterialAssetCSFLoader());
  mgr->RegisterLoader(new cs::MaterialDefAssetCSFLoader());
  mgr->RegisterLoader(new cs::MeshAssetCSFLoader());
  mgr->RegisterLoader(new cs::PropertySetterCSFLoader());
  mgr->RegisterLoader(new cs::SamplerAssetCSFLoader());
  mgr->RegisterLoader(new cs::ShaderGraphAssetCSFLoader());
  mgr->RegisterLoader(new cs::SkinnedMeshAssetCSFLoader());
  mgr->RegisterLoader(new cs::TextureAssetCSFLoader());


  // image loaders
  mgr->RegisterLoader(new cs::PNGImageAssetLoader());
  mgr->RegisterLoader(new cs::PNGImageFileLoader());
  mgr->RegisterLoader(new cs::SubMeshAssetLoader());

}
