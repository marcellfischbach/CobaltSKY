

#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.hh>


vkEntity::vkEntity()
  : vkObject ()
  , m_rootState(0)
{

}


vkEntity::~vkEntity()
{

}



void vkEntity::AddState(vkEntityState *state)
{
  if (state)
  {
    state->AddRef();
    m_states.push_back(state);
  }
}

void vkEntity::AddState(vkSpatialState *state, vkSpatialState *parentState)
{
  AddState(state);

  if (!parentState)
  {
    VK_SET(m_rootState, state);
  }
  else
  {
    parentState->AddSpatialState(state);
  }
}

