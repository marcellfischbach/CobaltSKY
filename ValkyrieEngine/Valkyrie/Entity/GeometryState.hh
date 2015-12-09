#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/GeometryState.refl.hh>


VK_CLASS()
class VKE_API vkGeometryState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkGeometryState();
  virtual ~vkGeometryState();


};