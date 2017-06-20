
#include <cobalt/entity/csentitystate.hh>


csEntityState::csEntityState()
  : csObject()
  , m_scene(0)
  , m_entity(0)
  , m_id (csID_Undefined)
  , m_created (false)
  , m_assembled(false)
{

}

csEntityState::~csEntityState()
{

}

csSpatialState *csEntityState::ToSpatialState()
{
  return 0;
}

const csSpatialState *csEntityState::ToSpatialState() const
{
  return 0;
}


void csEntityState::Create()
{
  if (m_created)
  {
    return;
  }
  m_created = true;
  OnCreated();
}

void csEntityState::Assemble()
{
  if (m_assembled)
  {
    return;
  }
  m_assembled = true;
  OnAssembled();
}

void csEntityState::AttachToEntity(csEntity *entity)
{
  if (m_entity)
  {
    return;
  }
  m_entity = entity;
  OnAttachedToEntity(entity);
}

void csEntityState::AttachToScene(csEntityScene *scene)
{
  if (m_scene || !scene)
  {
    return;
  }

  m_scene = scene;
  OnAttachedToScene(scene);
}

void csEntityState::DetachFromScene(csEntityScene *scene)
{
  if (!m_scene || m_scene != scene)
  {
    return;
  }
  m_scene = 0;
  OnDetachedFromScene(scene);
}

void csEntityState::DetachFromEntity(csEntity *entity)
{
  if (!m_entity || m_entity != entity)
  {
    return;
  }

  m_entity = 0;
  OnDetachedFromEntity(entity);
}

void csEntityState::Disassemble()
{
  if (!m_assembled)
  {
    return;
  }
  m_assembled = false;
  OnDisassembled();
}

void csEntityState::Destroy()
{
  if (!m_created)
  {
    return;
  }

  m_created = false;
  OnDestroyed();
}



void csEntityState::OnCreated()
{

}

void csEntityState::OnAssembled()
{

}

void csEntityState::OnAttachedToEntity(csEntity *entity)
{
}


void csEntityState::OnAttachedToScene(csEntityScene *scene)
{

}

void csEntityState::OnDetachedFromScene(csEntityScene *scene)
{

}


void csEntityState::OnDetachedFromEntity(csEntity *entity)
{
}

void csEntityState::OnDisassembled()
{

}

void csEntityState::OnDestroyed()
{

}

void csEntityState::Update(float tpf)
{

}

