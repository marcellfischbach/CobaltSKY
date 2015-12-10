
#include <Valkyrie/Entity/SpatialState.hh>

vkSpatialState::vkSpatialState()
  : vkEntityState()
{

}

vkSpatialState::~vkSpatialState()
{

}

vkTransformation vkSpatialState::GetTransformation()
{
  return vkTransformation(m_localMatrix, m_globalMatrix, 0);
}

void vkSpatialState::FinishTransformation()
{
  if (m_parentState)
  {
    vkMatrix4f::Mult(m_parentState->GetGlobalTransformation(), m_localMatrix, m_globalMatrix);
  }
  else
  {
    m_globalMatrix = m_localMatrix;
  }

  for (vkSize i = 0, in = m_childStates.size(); i < in; ++i)
  {
    m_childStates[i]->FinishTransformation();
  }
}

const vkMatrix4f &vkSpatialState::GetLocalTransformation() const
{
  return m_localMatrix;
}

const vkMatrix4f &vkSpatialState::GetGlobalTransformation() const
{
  return m_globalMatrix;
}



void vkSpatialState::AddSpatialState(vkSpatialState *state)
{
  if (state)
  {
    state->AddRef();
    state->m_parentState = this;
    m_childStates.push_back(state);
  }
}

void vkSpatialState::RemoveSpatialState(vkSpatialState *state)
{
  if (state)
  {
    for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
    {
      if (m_childStates[i] == state)
      {
        m_childStates.erase(m_childStates.begin() + i);
        state->Release();
        return;
      }
    }
  }
}



void vkSpatialState::Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan)
{
  PrivScan(clipper, graphics, entityScan);

  for (vkSize i = 0, in = m_childStates.size(); i < in; ++i)
  {
    m_childStates[i]->Scan(clipper, graphics, entityScan);
  }
}

void vkSpatialState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan)
{

}