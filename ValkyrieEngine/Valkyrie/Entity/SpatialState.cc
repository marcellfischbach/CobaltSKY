
#include <Valkyrie/Entity/SpatialState.hh>

vkSpatialState::vkSpatialState()
  : vkEntityState()
{

}

vkSpatialState::~vkSpatialState()
{

}

void vkSpatialState::AddSpatialState(vkSpatialState *state)
{
  if (state)
  {
    state->AddRef();
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


