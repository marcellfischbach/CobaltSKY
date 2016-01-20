
#include <Valkyrie/Entity/EntityState.hh>


vkEntityState::vkEntityState()
  : vkObject()
  , m_entity(0)
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



void vkEntityState::OnAttachedToEntity(vkEntity *entity)
{
  m_entity = entity;
}

void vkEntityState::OnDetachedFromEntity(vkEntity *entity)
{
  if (m_entity == entity)
  {
    m_entity = 0;
  }
}

void vkEntityState::OnAttachedToScene(vkEntityScene *scene)
{

}

void vkEntityState::OnDetachedFromScene(vkEntityScene *scene)
{

}


