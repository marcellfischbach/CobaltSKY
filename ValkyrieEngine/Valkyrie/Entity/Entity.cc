

#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.hh>

vkID nextID = 1;

vkID vkEntity::GetNextID()
{
  return nextID++;
}

vkEntity::vkEntity()
  : vkObject ()
  , m_rootState(0)
  , m_id(vkEntity::GetNextID())
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

void vkEntity::Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan)
{
  if (m_rootState)
  {
    m_rootState->Scan(clipper, graphics, entityScan);
  }
}