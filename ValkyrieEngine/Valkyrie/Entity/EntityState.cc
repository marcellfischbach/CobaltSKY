
#include <Valkyrie/Entity/EntityState.hh>


vkEntityState::vkEntityState()
  : vkObject()
  , m_scene(0)
  , m_entity(0)
  , m_id (vkID_Undefined)
  , m_created (false)
  , m_assembled(false)
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


void vkEntityState::Create()
{
  if (m_created)
  {
    return;
  }
  m_created = true;
  OnCreated();
}

void vkEntityState::Assemble()
{
  if (m_assembled)
  {
    return;
  }
  m_assembled = true;
  OnAssembled();
}

void vkEntityState::AttachToEntity(vkEntity *entity)
{
  if (m_entity)
  {
    return;
  }
  m_entity = entity;
  OnAttachedToEntity(entity);
}

void vkEntityState::AttachToScene(vkEntityScene *scene)
{
  if (m_scene || !scene)
  {
    return;
  }

  m_scene = scene;
  OnAttachedToScene(scene);
}

void vkEntityState::DetachFromScene(vkEntityScene *scene)
{
  if (!m_scene || m_scene != scene)
  {
    return;
  }
  m_scene = 0;
  OnDetachedFromScene(scene);
}

void vkEntityState::DetachFromEntity(vkEntity *entity)
{
  if (!m_entity || m_entity != entity)
  {
    return;
  }

  m_entity = 0;
  OnDetachedFromEntity(entity);
}

void vkEntityState::Disassemble()
{
  if (!m_assembled)
  {
    return;
  }
  m_assembled = false;
  OnDisassembled();
}

void vkEntityState::Destroy()
{
  if (!m_created)
  {
    return;
  }

  m_created = false;
  OnDestroyed();
}



void vkEntityState::OnCreated()
{

}

void vkEntityState::OnAssembled()
{

}

void vkEntityState::OnAttachedToEntity(vkEntity *entity)
{
}


void vkEntityState::OnAttachedToScene(vkEntityScene *scene)
{

}

void vkEntityState::OnDetachedFromScene(vkEntityScene *scene)
{

}


void vkEntityState::OnDetachedFromEntity(vkEntity *entity)
{
}

void vkEntityState::OnDisassembled()
{

}

void vkEntityState::OnDestroyed()
{

}

void vkEntityState::Update(float tpf)
{

}

