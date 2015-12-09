#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Entity/Entity.refl.hh>
#include <vector>

class vkEntityState;
class vkSpatialState;

VK_CLASS()
class VKE_API vkEntity : public vkObject
{
  VK_CLASS_GEN;
public:
  vkEntity();
  virtual ~vkEntity();

  void AddState(vkEntityState *state);
  void AddState(vkSpatialState *state, vkSpatialState *parentState);

private:

  vkSpatialState *m_rootState;

  std::vector<vkEntityState*> m_states;
};