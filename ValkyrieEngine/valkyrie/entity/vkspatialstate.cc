
#include <valkyrie/entity/vkspatialstate.hh>
#include <valkyrie/entity/ientityscan.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/math/vkclipper.hh>
#include <valkyrie/vktime.hh>
#include <valkyrie/entity/vkcolliderstate.hh>

vkSpatialState::vkSpatialState()
  : vkEntityState()
  , m_parentState(0)
  , m_boundingBoxDirty(true)
{

}

vkSpatialState::~vkSpatialState()
{

}

vkSpatialState *vkSpatialState::ToSpatialState()
{
  return this;
}

const vkSpatialState *vkSpatialState::ToSpatialState() const
{
  return this;
}



vkTransformation vkSpatialState::GetTransformation()
{
  if (m_parentState)
  {
    return vkTransformation(&m_localMatrix, &m_globalMatrix, &m_parentState->m_globalMatrix, &m_parentState->m_globalMatrixInv);
  }
  else
  {
    return vkTransformation(&m_localMatrix, &m_globalMatrix, 0, 0);
  }
}

void vkSpatialState::FinishTransformation()
{
  PerformTransformation();
}


void vkSpatialState::PerformTransformation()
{
  // make propagate the transformation to the children
  // this will also update the bounding box of all those child states (incl. this one)
  UpdateTransformation();

  // traverse up to the root state 
  if (m_parentState)
  {
    m_parentState->FlagBoundingBoxDirty();
  }
}

void vkSpatialState::UpdateTransformation()
{
  if (m_parentState)
  {
    vkMatrix4f::Mult(m_parentState->GetGlobalTransformation(), m_localMatrix, m_globalMatrix);
  }
  else
  {
    m_globalMatrix = m_localMatrix;
  }

  m_globalMatrix.Inverted(m_globalMatrixInv);

  m_boundingBoxDirty = true;

  for (vkSize i = 0, in = m_childStates.size(); i < in; ++i)
  {
    m_childStates[i]->UpdateTransformation();
  }

}

void vkSpatialState::FlagParentBoundingBoxDirty()
{
  if (m_parentState)
  {
    m_parentState->FlagBoundingBoxDirty();
  }
}

void vkSpatialState::FlagBoundingBoxDirty()
{
  if (m_boundingBoxDirty)
  {
    return;
  }

  m_boundingBoxDirty = true;
  if (m_parentState)
  {
    m_parentState->FlagBoundingBoxDirty();
  }
}

void vkSpatialState::SetClippingRange(float min, float max)
{
  m_distanceState.SetupDistance(min, max);

}

void vkSpatialState::UpdateBoundingBox()
{
  if (!m_boundingBoxDirty)
  {
    return;
  }
  m_boundingBox.Clear();

  FillBoundingBox(m_boundingBox);

  m_boundingBox.Finish();
}

void vkSpatialState::FillBoundingBox(vkBoundingBox &bbox)
{
  for (vkSize i = 0, in = GetNumberOfChildState(); i < in; ++i)
  {
    vkSpatialState *state = GetChildState(i);
    if (state)
    {
      state->UpdateBoundingBox();
      bbox.Add(state->GetBoundingBox());
    }
  }
}

vkSize vkSpatialState::GetNumberOfChildState() const
{
  return m_childStates.size();
}

vkSpatialState *vkSpatialState::GetChildState(vkSize idx)
{
  if (idx >= m_childStates.size())
  {
    return 0;
  }

  return m_childStates[idx];
}

const vkSpatialState *vkSpatialState::GetChildState(vkSize idx) const
{
  if (idx >= m_childStates.size())
  {
    return 0;
  }

  return m_childStates[idx];
}

void vkSpatialState::AddSpatialState(vkSpatialState *state)
{
  if (state)
  {
    state->AddRef();
    state->m_parentState = this;
    m_childStates.push_back(state);
    FlagBoundingBoxDirty();
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
        state->m_parentState = 0;
        state->Release();
        FlagBoundingBoxDirty();
        return;
      }
    }
  }
}

vkSpatialState *vkSpatialState::FindState(const vkString &stateName)
{
  if (stateName == GetName())
  {
    return this;
  }

  for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
  {
    vkSpatialState *state = m_childStates[i]->FindState(stateName);
    if (state)
    {
      return state;
    }
  }
  return 0;
}

const vkSpatialState *vkSpatialState::FindState(const vkString &stateName) const
{
  if (stateName == GetName())
  {
    return this;
  }

  for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
  {
    vkSpatialState *state = m_childStates[i]->FindState(stateName);
    if (state)
    {
      return state;
    }
  }
  return 0;
}


void vkSpatialState::Scan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config)
{
  if (clipper && false)
  {
    vkClipper::ClipResult cr = clipper->Test(m_boundingBox);
    switch (cr)
    {
    case vkClipper::eCR_In:
      // we are completely within the viewing frustum no need to test states below.
      clipper = 0;
      break;

    case vkClipper::eCR_Out:
      // we are out... nothin to clip
      return;

    case vkClipper::eCR_Intermediate:
      // we are partially within the viewing frustum.. the entity will be rendered, but states below 
      // must be tested aswell.
      break;
    }
  }

  vkVector3f dir;
  vkVector3f::Sub(config.MainCameraPosition, m_boundingBox.GetCenter(), dir);
  float distanceSqr = dir.Dot();

  m_distanceState.UpdateDistance(distanceSqr);
  if (m_distanceState.IsOut())
  {
    return;
  }


  PrivScan(clipper, graphics, entityScan, config);

  for (vkSize i = 0, in = m_childStates.size(); i < in; ++i)
  {
    m_childStates[i]->Scan(clipper, graphics, entityScan, config);
  }
}

void vkSpatialState::PrivScan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config)
{

}





#define VK_NUM_FADE_VALUES 18
#define VK_FADE_VALUE_OUT 0
#define VK_FADE_VALUE_FIRST_IN 1
#define VK_FADE_VALUE_IN 17
#define VK_FADE_VALUE_FIRST_OUT 16


vkDistanceState::vkDistanceState()
  : m_min (-FLT_MAX)
  , m_max (FLT_MAX)
  , m_state(eInitial)
{
  SetFadeTime(500L);
}

void vkDistanceState::SetupDistance(float min, float max)
{
  m_min = min;
  m_max = max;
  if (m_min != -FLT_MAX && m_min != FLT_MAX)
  {
    m_min *= m_min;
  }
  if (m_max != -FLT_MAX && m_max != FLT_MAX)
  {
    m_max *= m_max;
  }
}

void vkDistanceState::SetupState(State state, vkUInt8 fadeValue)
{
  m_fadeValue = fadeValue;
  m_state = state;
}

void vkDistanceState::SetFadeTime(vkUInt64 fadeTimeMilli)
{
  m_fadeTimeMicroPerValue = fadeTimeMilli * 1000L / (vkUInt64)VK_NUM_FADE_VALUES;
}

void vkDistanceState::UpdateDistance(float distance)
{
  bool in = m_min <= distance && distance <= m_max;
  vkUInt64 ct = vkTime::Get().GetCurrentTimeMicro();

  switch (m_state)
  {
  case eInitial:
    if (in)
    {
      m_state = eIn;
      m_fadeValue = VK_FADE_VALUE_IN;
    }
    else
    {
      m_state = eOut;
      m_fadeValue = VK_FADE_VALUE_OUT;
    }
    break;


  case eOut:
    if (in)
    {
      m_state = eFadeIn;
      m_nextFadeValueChange = ct + m_fadeTimeMicroPerValue;
      m_fadeValue = VK_FADE_VALUE_FIRST_IN;
    }
    break;
  case eFadeIn:
    if (in)
    {
      if (ct >= m_nextFadeValueChange)
      {
        m_fadeValue++;
        if (m_fadeValue == VK_FADE_VALUE_IN)
        {
          m_state = eIn;
        }
        else
        {
          m_nextFadeValueChange = ct + m_fadeTimeMicroPerValue;
        }
      }
    }
    else
    {
      m_state = eFadeOut;
      m_nextFadeValueChange = ct + m_fadeTimeMicroPerValue;
    }
    break;

  case eIn:
    if (!in)
    {
      m_state = eFadeOut;
      m_nextFadeValueChange = vkTime::Get().GetCurrentTimeMicro() + m_fadeTimeMicroPerValue;
      m_fadeValue = VK_FADE_VALUE_FIRST_OUT;
    }
    break;
  case eFadeOut:
    if (!in)
    {
      vkUInt64 ct = vkTime::Get().GetCurrentTimeMicro();
      if (ct >= m_nextFadeValueChange)
      {
        m_fadeValue--;
        if (m_fadeValue == VK_FADE_VALUE_OUT)
        {
          m_state = eOut;
        }
        else
        {
          m_nextFadeValueChange = ct + m_fadeTimeMicroPerValue;
        }
      }
    }
    else
    {
      m_state = eFadeIn;
      m_nextFadeValueChange = ct + m_fadeTimeMicroPerValue;
    }
    break;

  }

}
