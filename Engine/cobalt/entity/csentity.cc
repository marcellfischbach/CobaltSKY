 

#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/entity/cstransformstate.hh>
#include <cobalt/physics/iphysicscollider.hh>
#include <cobalt/csengine.hh>

csID nextID = 1;

csID cs::Entity::GetNextID()
{
  return nextID++;
}

cs::Entity::Entity()
  : cs::Object ()
  , m_rootState(0)
  , m_created(false)
  , m_assemabled(false)
  , m_scene(0)
  , m_parentEntity(0)
  , m_id(cs::Entity::GetNextID())
{

}


cs::Entity::~Entity()
{

}

cs::EntityState *cs::Entity::GetState(csID id)
{
  if (id == csID_Undefined)
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    cs::EntityState *state = m_states[i];
    if (state && state->GetId() == id)
    {
      return state;
    }
  }
  return 0;
}


const cs::EntityState *cs::Entity::GetState(csID id) const
{
  if (id == csID_Undefined)
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    cs::EntityState *state = m_states[i];
    if (state && state->GetId() == id)
    {
      return state;
    }
  }
  return 0;
}

cs::EntityState *cs::Entity::GetState(const std::string &name)
{
  if (name == "")
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    cs::EntityState *state = m_states[i];
    if (state && state->GetName() == name)
    {
      return state;
    }
  }
  return 0;
}


const cs::EntityState *cs::Entity::GetState(const std::string &name) const
{
  if (name == "")
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    cs::EntityState *state = m_states[i];
    if (state && state->GetName() == name)
    {
      return state;
    }
  }
  return 0;
}

std::vector<cs::EntityState*> cs::Entity::FindStates(const cs::Class* cls) const
{
  std::vector<cs::EntityState*> result;

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    cs::EntityState *state = m_states[i];
    if (state->GetClass()->IsInstanceOf(cls))
    {
      result.push_back(state);
    }
  }

  return result;
}

cs::EntityState* cs::Entity::FindState(const cs::Class* cls) const
{
  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    cs::EntityState *state = m_states[i];
    if (state->GetClass()->IsInstanceOf(cls))
    {
      return state;
    }
  }
  return 0;
}


cs::Transformation cs::Entity::GetTransformation()
{
  if (m_rootState)
  {
    return m_rootState->GetTransformation();
  }

  return cs::Transformation();
}

void cs::Entity::FinishTransformation()
{
  if (m_rootState)
  {
    m_rootState->FinishTransformation();
  }
}

void cs::Entity::PerformTransformation()
{
  if (m_rootState)
  {
    m_rootState->PerformTransformation();
  }
}

void cs::Entity::UpdateBoundingBox()
{
  if (m_rootState)
  {
    m_rootState->UpdateBoundingBox();
  }
}

void cs::Entity::SetRootState(cs::SpatialState *rootState)
{
  CS_SET(m_rootState, rootState);

}

void cs::Entity::RemoveState(cs::EntityState *state)
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

void cs::Entity::AddState(cs::EntityState *state)
{
  if (!state)
  {
    return;
  }
  state->AddRef();
  m_states.push_back(state);
  state->AttachToEntity(this);
  state->AttachToScene(m_scene);
  m_assemabled = false;
}

void cs::Entity::AddState(cs::SpatialState *state, cs::SpatialState *parentState)
{
  AddState(state);

  if (parentState)
  {
    parentState->AddSpatialState(state);
  }
}

void cs::Entity::Scan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
{
  if (m_rootState)
  {
    m_rootState->Scan(clipper, graphics, entityScan, config);
  }
}

void cs::Entity::Update(float tpf)
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

const cs::BoundingBox& cs::Entity::GetBoundingBox() const
{
  return m_rootState->GetBoundingBox();
}



void cs::Entity::AttachEntity(cs::Entity *entity, cs::SpatialState *parentState)
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

void cs::Entity::AttachEntity(cs::Entity *entity, const std::string &parentStateName)
{
  if (!m_rootState)
  {
    return;
  }
  cs::SpatialState *parentState = m_rootState->FindState(parentStateName);
  AttachEntity(entity, parentState);
}

void cs::Entity::DetachEntity(cs::Entity *entity)
{
  if (!entity)
  {
    return;
  }

  if (entity->m_parentEntity == this && entity->m_rootState)
  {
    cs::SpatialState *parentState = entity->m_rootState->GetParentState();
    if (parentState)
    {
      parentState->RemoveSpatialState(entity->m_rootState);
    }
  }

}

void cs::Entity::SetClippingRange(float min, float max)
{
  if (m_rootState)
  {
    m_rootState->SetClippingRange(min, max);
  }
}



void cs::Entity::Create()
{
  if (m_created)
  {
    return;
  }
  m_created = true;
  OnCreated();
}

void cs::Entity::Assemble()
{
  if (m_assemabled)
  {
    return;
  }
  m_assemabled = true;
  OnAssembled();
}

void cs::Entity::AttachToEntity(cs::Entity *entity)
{
  if (m_parentEntity)
  {
    return;
  }
  m_parentEntity = entity;
  OnAttachedToEntity(entity);
}

void cs::Entity::AttachToScene(cs::EntityScene *scene)
{
  if (m_scene)
  {
    return;
  }

  m_scene = scene;
  OnAttachedToScene(scene);
}

void cs::Entity::DetachFromScene(cs::EntityScene* scene)
{
  if (m_scene != scene)
  {
    return;
  }

  m_scene = 0;
  OnDetachedFromScene(scene);
}

void cs::Entity::DetachFromEntity(cs::Entity *entity)
{
  if (m_parentEntity != entity)
  {
    return;
  }
  m_parentEntity = 0;
  OnDetachedFromEntity(entity);
}

void cs::Entity::Disassemble()
{
  if (!m_assemabled)
  {
    return;
  }
  m_assemabled = false;
  OnDisassembled();
}

void cs::Entity::Destroy()
{
  if (!m_created)
  { 
    return;
  }
  m_created = false;
  OnDestroyed();
}



void cs::Entity::OnCreated()
{

}

void cs::Entity::OnAssembled()
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

void cs::Entity::OnAttachedToEntity(cs::Entity *entity)
{

}

void cs::Entity::OnAttachedToScene(cs::EntityScene *scene)
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

void cs::Entity::OnDetachedFromScene(cs::EntityScene *scene)
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

void cs::Entity::OnDetachedFromEntity(cs::Entity *entity)
{

}

void cs::Entity::OnDisassembled()
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

void cs::Entity::OnDestroyed()
{

}
