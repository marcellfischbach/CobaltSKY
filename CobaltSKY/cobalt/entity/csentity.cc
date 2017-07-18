

#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/csengine.hh>

csID nextID = 1;

csID csEntity::GetNextID()
{
  return nextID++;
}

csEntity::csEntity()
  : csObject ()
  , m_rootState(0)
  , m_created(false)
  , m_assemabled(false)
  , m_scene(0)
  , m_parentEntity(0)
  , m_id(csEntity::GetNextID())
{

}


csEntity::~csEntity()
{

}

csEntityState *csEntity::GetState(csID id)
{
  if (id == csID_Undefined)
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    csEntityState *state = m_states[i];
    if (state && state->GetID() == id)
    {
      return state;
    }
  }
  return 0;
}


const csEntityState *csEntity::GetState(csID id) const
{
  if (id == csID_Undefined)
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    csEntityState *state = m_states[i];
    if (state && state->GetID() == id)
    {
      return state;
    }
  }
  return 0;
}

csEntityState *csEntity::GetState(const csString &name)
{
  if (name == "")
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    csEntityState *state = m_states[i];
    if (state && state->GetName() == name)
    {
      return state;
    }
  }
  return 0;
}


const csEntityState *csEntity::GetState(const csString &name) const
{
  if (name == "")
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    csEntityState *state = m_states[i];
    if (state && state->GetName() == name)
    {
      return state;
    }
  }
  return 0;
}

std::vector<csEntityState*> csEntity::FindStates(const csClass* cls) const
{
  std::vector<csEntityState*> result;

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    csEntityState *state = m_states[i];
    if (state->GetClass()->IsInstanceOf(cls))
    {
      result.push_back(state);
    }
  }

  return result;
}

csEntityState* csEntity::FindState(const csClass* cls) const
{
  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    csEntityState *state = m_states[i];
    if (state->GetClass()->IsInstanceOf(cls))
    {
      return state;
    }
  }
  return 0;
}


csTransformation csEntity::GetTransformation()
{
  if (m_rootState)
  {
    return m_rootState->GetTransformation();
  }

  return csTransformation();
}

void csEntity::FinishTransformation()
{
  if (m_rootState)
  {
    m_rootState->FinishTransformation();
  }
}

void csEntity::PerformTransformation()
{
  if (m_rootState)
  {
    m_rootState->PerformTransformation();
  }
}

void csEntity::UpdateBoundingBox()
{
  if (m_rootState)
  {
    m_rootState->UpdateBoundingBox();
  }
}

void csEntity::SetRootState(csSpatialState *rootState)
{
  CS_SET(m_rootState, rootState);

}

void csEntity::RemoveState(csEntityState *state)
{
  if (state->GetEntity() != this)
  {
    // this state is not attached to this entity
    return;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    if (m_states[i] == state)
    {
      m_states.erase(m_states.begin() + i);
      break;
    }
  }
  if (m_rootState == state)
  {
    CS_RELEASE(m_rootState);
  }
  state->DetachFromEntity(this);
  state->DetachFromScene(m_scene);
  m_assemabled = false;

  state->Release();
}

void csEntity::AddState(csEntityState *state)
{
  if (state)
  {
    state->AddRef();
    m_states.push_back(state);
    state->AttachToEntity(this);
    state->AttachToScene(m_scene);
    m_assemabled = false;
  }
}

void csEntity::AddState(csSpatialState *state, csSpatialState *parentState)
{
  AddState(state);

  if (parentState)
  {
    parentState->AddSpatialState(state);
  }
}

void csEntity::Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  if (m_rootState)
  {
    m_rootState->Scan(clipper, graphics, entityScan, config);
  }
}

void csEntity::Update(float tpf)
{
  for (csSize i = 0, in = m_states.size(); i < in; ++i)
  {
    m_states[i]->Update(tpf);
  }
  for (csSize i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Update(tpf);
  }
}

const csBoundingBox& csEntity::GetBoundingBox() const
{
  return m_rootState->GetBoundingBox();
}



void csEntity::AttachEntity(csEntity *entity, csSpatialState *parentState)
{
  if (!entity)
  {
    return;
  }
  if (!parentState)
  {
    parentState = m_rootState;
  }
  if (!parentState)
  {
    return;
  }
  parentState->AddSpatialState(entity->m_rootState);

  entity->AddRef();
  m_children.push_back(entity);

  entity->AttachToEntity(this);
  entity->AttachToScene(m_scene);
}

void csEntity::AttachEntity(csEntity *entity, const csString &parentStateName)
{
  if (!m_rootState)
  {
    return;
  }
  csSpatialState *parentState = m_rootState->FindState(parentStateName);
  AttachEntity(entity, parentState);
}

void csEntity::DetachEntity(csEntity *entity)
{
  if (!entity)
  {
    return;
  }

  if (entity->m_parentEntity == this && entity->m_rootState)
  {
    csSpatialState *parentState = entity->m_rootState->GetParentState();
    if (parentState)
    {
      parentState->RemoveSpatialState(entity->m_rootState);
    }
  }

}

void csEntity::SetClippingRange(float min, float max)
{
  if (m_rootState)
  {
    m_rootState->SetClippingRange(min, max);
  }
}



void csEntity::Create()
{
  if (m_created)
  {
    return;
  }
  m_created = true;
  OnCreated();
}

void csEntity::Assemble()
{
  if (m_assemabled)
  {
    return;
  }
  m_assemabled = true;
  OnAssembled();
}

void csEntity::AttachToEntity(csEntity *entity)
{
  if (m_parentEntity)
  {
    return;
  }
  m_parentEntity = entity;
  OnAttachedToEntity(entity);
}

void csEntity::AttachToScene(csEntityScene *scene)
{
  if (m_scene)
  {
    return;
  }

  m_scene = scene;
  OnAttachedToScene(scene);
}

void csEntity::DetachFromScene(csEntityScene* scene)
{
  if (m_scene != scene)
  {
    return;
  }

  m_scene = 0;
  OnDetachedFromScene(scene);
}

void csEntity::DetachFromEntity(csEntity *entity)
{
  if (m_parentEntity != entity)
  {
    return;
  }
  m_parentEntity = 0;
  OnDetachedFromEntity(entity);
}

void csEntity::Disassemble()
{
  if (!m_assemabled)
  {
    return;
  }
  m_assemabled = false;
  OnDisassembled();
}

void csEntity::Destroy()
{
  if (!m_created)
  { 
    return;
  }
  m_created = false;
  OnDestroyed();
}



void csEntity::OnCreated()
{

}

void csEntity::OnAssembled()
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Assemble();
  }
  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    m_states[i]->Assemble();
  }
}

void csEntity::OnAttachedToEntity(csEntity *entity)
{

}

void csEntity::OnAttachedToScene(csEntityScene *scene)
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->AttachToScene(scene);
  }
  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    m_states[i]->AttachToScene(m_scene);
  }
}

void csEntity::OnDetachedFromScene(csEntityScene *scene)
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->DetachFromScene(scene);
  }
  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    m_states[i]->DetachFromScene(m_scene);
  }
}

void csEntity::OnDetachedFromEntity(csEntity *entity)
{

}

void csEntity::OnDisassembled()
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Assemble();
  }
  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    m_states[i]->Assemble();
  }
}

void csEntity::OnDestroyed()
{

}
