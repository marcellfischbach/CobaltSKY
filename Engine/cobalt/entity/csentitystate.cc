
#include <cobalt/entity/csentitystate.hh>


cs::EntityState::EntityState()
  : cs::Object()
  , m_scene(0)
  , m_entity(0)
  , m_id (csID_Undefined)
  , m_created (false)
  , m_assembled(false)
{

}

void cs::EntityState::SetId(csID id)
{
  m_id = id;
}

cs::EntityState::~EntityState()
{

}

cs::SpatialState *cs::EntityState::ToSpatialState()
{
  return 0;
}

const cs::SpatialState *cs::EntityState::ToSpatialState() const
{
  return 0;
}


void cs::EntityState::Create()
{
  if (m_created)
  {
    return;
  }
  m_created = true;
  OnCreated();
}

void cs::EntityState::Assemble()
{
  if (m_assembled)
  {
    return;
  }
  m_assembled = true;
  OnAssembled();
}

void cs::EntityState::AttachToEntity(cs::Entity *entity)
{
  if (m_entity)
  {
    return;
  }
  m_entity = entity;
  OnAttachedToEntity(entity);
}

void cs::EntityState::AttachToScene(cs::EntityScene *scene)
{
  if (m_scene || !scene)
  {
    return;
  }

  m_scene = scene;
  OnAttachedToScene(scene);
}

void cs::EntityState::DetachFromScene(cs::EntityScene *scene)
{
  if (!m_scene || m_scene != scene)
  {
    return;
  }
  m_scene = 0;
  OnDetachedFromScene(scene);
}

void cs::EntityState::DetachFromEntity(cs::Entity *entity)
{
  if (!m_entity || m_entity != entity)
  {
    return;
  }

  m_entity = 0;
  OnDetachedFromEntity(entity);
}

void cs::EntityState::Disassemble()
{
  if (!m_assembled)
  {
    return;
  }
  m_assembled = false;
  OnDisassembled();
}

void cs::EntityState::Destroy()
{
  if (!m_created)
  {
    return;
  }

  m_created = false;
  OnDestroyed();
}



void cs::EntityState::OnCreated()
{

}

void cs::EntityState::OnAssembled()
{

}

void cs::EntityState::OnAttachedToEntity(cs::Entity *entity)
{
}


void cs::EntityState::OnAttachedToScene(cs::EntityScene *scene)
{

}

void cs::EntityState::OnDetachedFromScene(cs::EntityScene *scene)
{

}


void cs::EntityState::OnDetachedFromEntity(cs::Entity *entity)
{
}

void cs::EntityState::OnDisassembled()
{

}

void cs::EntityState::OnDestroyed()
{

}

void cs::EntityState::Update(float tpf)
{

}

