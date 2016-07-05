

#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Physics/IPhysicsCollider.hh>
#include <Valkyrie/Engine.hh>

vkID nextID = 1;

vkID vkEntity::GetNextID()
{
  return nextID++;
}

vkEntity::vkEntity()
  : vkObject ()
  , m_rootState(0)
  , m_created(false)
  , m_assemabled(false)
  , m_scene(0)
  , m_parentEntity(0)
  , m_id(vkEntity::GetNextID())
{

}


vkEntity::~vkEntity()
{

}

vkEntityState *vkEntity::GetState(vkID id)
{
  if (id == vkID_Undefined)
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    vkEntityState *state = m_states[i];
    if (state && state->GetID() == id)
    {
      return state;
    }
  }
  return 0;
}


const vkEntityState *vkEntity::GetState(vkID id) const
{
  if (id == vkID_Undefined)
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    vkEntityState *state = m_states[i];
    if (state && state->GetID() == id)
    {
      return state;
    }
  }
  return 0;
}

vkEntityState *vkEntity::GetState(const vkString &name)
{
  if (name == "")
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    vkEntityState *state = m_states[i];
    if (state && state->GetName() == name)
    {
      return state;
    }
  }
  return 0;
}


const vkEntityState *vkEntity::GetState(const vkString &name) const
{
  if (name == "")
  {
    return 0;
  }

  for (size_t i = 0, in = m_states.size(); i < in; ++i)
  {
    vkEntityState *state = m_states[i];
    if (state && state->GetName() == name)
    {
      return state;
    }
  }
  return 0;
}


vkTransformation vkEntity::GetTransformation()
{
  if (m_rootState)
  {
    return m_rootState->GetTransformation();
  }

  return vkTransformation();
}

void vkEntity::FinishTransformation()
{
  if (m_rootState)
  {
    m_rootState->FinishTransformation();
  }
}

void vkEntity::PerformTransformation()
{
  if (m_rootState)
  {
    m_rootState->PerformTransformation();
  }
}

void vkEntity::UpdateBoundingBox()
{
  if (m_rootState)
  {
    m_rootState->UpdateBoundingBox();
  }
}

void vkEntity::SetRootState(vkSpatialState *rootState)
{
  VK_SET(m_rootState, rootState);
}

void vkEntity::AddState(vkEntityState *state)
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

void vkEntity::AddState(vkSpatialState *state, vkSpatialState *parentState)
{
  AddState(state);

  if (parentState)
  {
    parentState->AddSpatialState(state);
  }
}

void vkEntity::Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_rootState)
  {
    m_rootState->Scan(clipper, graphics, entityScan, config);
  }
}

void vkEntity::Update(float tpf)
{
  for (vkSize i = 0, in = m_states.size(); i < in; ++i)
  {
    m_states[i]->Update(tpf);
  }
  for (vkSize i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Update(tpf);
  }
}

const vkBoundingBox& vkEntity::GetBoundingBox() const
{
  return m_rootState->GetBoundingBox();
}



void vkEntity::AttachEntity(vkEntity *entity, vkSpatialState *parentState)
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

void vkEntity::AttachEntity(vkEntity *entity, const vkString &parentStateName)
{
  if (!m_rootState)
  {
    return;
  }
  vkSpatialState *parentState = m_rootState->FindState(parentStateName);
  AttachEntity(entity, parentState);
}

void vkEntity::DetachEntity(vkEntity *entity)
{
  if (!entity)
  {
    return;
  }

  if (entity->m_parentEntity == this && entity->m_rootState)
  {
    vkSpatialState *parentState = entity->m_rootState->GetParentState();
    if (parentState)
    {
      parentState->RemoveSpatialState(entity->m_rootState);
    }
  }

}

void vkEntity::SetClippingRange(float min, float max)
{
  if (m_rootState)
  {
    m_rootState->SetClippingRange(min, max);
  }
}



void vkEntity::Create()
{
  if (m_created)
  {
    return;
  }
  m_created = true;
  OnCreated();
}

void vkEntity::Assemble()
{
  if (m_assemabled)
  {
    return;
  }
  m_assemabled = true;
  OnAssembled();
}

void vkEntity::AttachToEntity(vkEntity *entity)
{
  if (m_parentEntity)
  {
    return;
  }
  m_parentEntity = entity;
  OnAttachedToEntity(entity);
}

void vkEntity::AttachToScene(vkEntityScene *scene)
{
  if (m_scene)
  {
    return;
  }

  m_scene = scene;
  OnAttachedToScene(scene);
}

void vkEntity::DetachFromScene(vkEntityScene* scene)
{
  if (m_scene != scene)
  {
    return;
  }

  m_scene = 0;
  OnDetachedFromScene(scene);
}

void vkEntity::DetachFromEntity(vkEntity *entity)
{
  if (m_parentEntity != entity)
  {
    return;
  }
  m_parentEntity = 0;
  OnDetachedFromEntity(entity);
}

void vkEntity::Disassemble()
{
  if (!m_assemabled)
  {
    return;
  }
  m_assemabled = false;
  OnDisassembled();
}

void vkEntity::Destroy()
{
  if (!m_created)
  { 
    return;
  }
  m_created = false;
  OnDestroyed();
}



void vkEntity::OnCreated()
{

}

void vkEntity::OnAssembled()
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

void vkEntity::OnAttachedToEntity(vkEntity *entity)
{

}

void vkEntity::OnAttachedToScene(vkEntityScene *scene)
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

void vkEntity::OnDetachedFromScene(vkEntityScene *scene)
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

void vkEntity::OnDetachedFromEntity(vkEntity *entity)
{

}

void vkEntity::OnDisassembled()
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

void vkEntity::OnDestroyed()
{

}