
#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/Geometry.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Entity/Module.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Entity/RigidBodyState.hh>
#include <Valkyrie/Entity/SpatialState.hh>


void vkEntityModule::Initialize()
{
  vkClassRegistry *cr = vkClassRegistry::Get();

  cr->RegisterClass(vkEntity::GetStaticClass());
  cr->RegisterClass(vkEntityState::GetStaticClass());
  cr->RegisterClass(vkGeometryData::GetStaticClass());
  cr->RegisterClass(vkGeometryMesh::GetStaticClass());
  cr->RegisterClass(vkLightState::GetStaticClass());
  cr->RegisterClass(vkStaticMeshState::GetStaticClass());
  cr->RegisterClass(vkRenderState::GetStaticClass());
  cr->RegisterClass(vkSpatialState::GetStaticClass());
  cr->RegisterClass(vkRigidBodyState::GetStaticClass());
}
