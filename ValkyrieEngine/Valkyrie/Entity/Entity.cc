

#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Physics/IPhysicsBody.hh>
#include <Valkyrie/Engine.hh>

vkID nextID = 1;

vkID vkEntity::GetNextID()
{
  return nextID++;
}

vkEntity::vkEntity()
  : vkObject ()
  , m_rootState(0)
  , m_id(vkEntity::GetNextID())
  , m_collisionBody(0)
  , m_triggerBody(0)
{

}


vkEntity::~vkEntity()
{

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


    // update the physics system (if one is attached)
    if (m_collisionBody)
    {
      m_collisionBody->GetTransform().SetTransformation(m_rootState->GetGlobalTransformation());
      m_collisionBody->FinishTransformation();
    }
    if (m_triggerBody)
    {
      m_triggerBody->GetTransform().SetTransformation(m_rootState->GetGlobalTransformation());
      m_triggerBody->FinishTransformation();
    }
  }
}

void vkEntity::UpdatePhysicsTransformation()
{
  if (m_rootState)
  {
    if (m_collisionBody)
    {
      m_rootState->GetTransformation().SetTransformation(m_collisionBody->GetMatrix());
      m_rootState->FinishTransformation();
      
      if (m_triggerBody)
      {
        m_triggerBody->GetTransform().SetTransformation(m_rootState->GetGlobalTransformation());
      }
    }
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
    state->OnAttachedToEntity(this);
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
  entity->m_parentEntity = this;
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

IPhysicsBody *vkEntity::AcquireCollisionBody()
{
  if (!m_collisionBody)
  {
    m_collisionBody = vkEngine::Get()->GetPhysicsSystem()->CreateBody();
    m_collisionBody->SetEntity(this);
  }
  return m_collisionBody;
}