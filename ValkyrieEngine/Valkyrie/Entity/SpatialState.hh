#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.refl.hh>
#include <vector>


VK_CLASS()
class VKE_API vkSpatialState : public vkEntityState
{
  friend class vkEntity;
  VK_CLASS_GEN;

public:
  vkSpatialState();
  virtual ~vkSpatialState();

private:
  void AddSpatialState(vkSpatialState *state);
  void RemoveSpatialState(vkSpatialState *state);

private:
  std::vector<vkSpatialState*> m_childStates;
};