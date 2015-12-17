
#include <Valkyrie/Entity/EntityState.hh>


vkEntityState::vkEntityState()
  : vkObject()
{

}

vkEntityState::~vkEntityState()
{

}


vkSpatialState *vkEntityState::ToSpatialState()
{
  return 0;
}

const vkSpatialState *vkEntityState::ToSpatialState() const
{
  return 0;
}

