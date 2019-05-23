
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/math/csclipper.hh>
#include <cobalt/cstime.hh>
#include <cobalt/entity/cscolliderstate.hh>

cs::SpatialState::SpatialState()
  : cs::EntityState()
  , m_parentState(0)
  , m_boundingBoxDirty(true)
{

}

cs::SpatialState::~SpatialState()
{

}

cs::SpatialState *cs::SpatialState::ToSpatialState()
{
  return this;
}

const cs::SpatialState *cs::SpatialState::ToSpatialState() const
{
  return this;
}



cs::Transformation cs::SpatialState::GetTransformation()
{
  if (m_parentState)
  {
    return cs::Transformation(&m_localMatrix, &m_globalMatrix, &m_parentState->m_globalMatrix, &m_parentState->m_globalMatrixInv);
  }
  else
  {
    return cs::Transformation(&m_localMatrix, &m_globalMatrix, 0, 0);
  }
}

void cs::SpatialState::FinishTransformation()
{
  PerformTransformation();
}

void cs::SpatialState::SetLocalMatrix(const cs::Matrix4f &localMatrix)
{
  m_localMatrix.Set(localMatrix);
}

const cs::Matrix4f &cs::SpatialState::GetLocalMatrix() const
{
  return m_localMatrix;
}

void cs::SpatialState::PerformTransformation()
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

void cs::SpatialState::UpdateTransformation()
{
  if (m_parentState)
  {
    cs::Matrix4f::Mult(m_parentState->GetGlobalTransformation(), m_localMatrix, m_globalMatrix);
  }
  else
  {
    m_globalMatrix = m_localMatrix;
  }

  m_globalMatrix.Inverted(m_globalMatrixInv);

  m_boundingBoxDirty = true;

  for (csSize i = 0, in = m_childStates.size(); i < in; ++i)
  {
    m_childStates[i]->UpdateTransformation();
  }

}

void cs::SpatialState::FlagParentBoundingBoxDirty()
{
  if (m_parentState)
  {
    m_parentState->FlagBoundingBoxDirty();
  }
}

void cs::SpatialState::FlagBoundingBoxDirty()
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

void cs::SpatialState::SetClippingRange(float min, float max)
{
  m_distanceState.SetupDistance(min, max);

}

void cs::SpatialState::UpdateBoundingBox()
{
  if (!m_boundingBoxDirty)
  {
    return;
  }
  m_boundingBox.Clear();

  FillBoundingBox(m_boundingBox);

  m_boundingBox.Finish();
}

void cs::SpatialState::FillBoundingBox(cs::BoundingBox &bbox)
{
  for (csSize i = 0, in = GetNumberOfChildState(); i < in; ++i)
  {
    cs::SpatialState *state = GetChildState(i);
    if (state)
    {
      state->UpdateBoundingBox();
      bbox.Add(state->GetBoundingBox());
    }
  }
}

csSize cs::SpatialState::GetNumberOfChildState() const
{
  return m_childStates.size();
}

cs::SpatialState *cs::SpatialState::GetChildState(csSize idx)
{
  if (idx >= m_childStates.size())
  {
    return 0;
  }

  return m_childStates[idx];
}

const cs::SpatialState *cs::SpatialState::GetChildState(csSize idx) const
{
  if (idx >= m_childStates.size())
  {
    return 0;
  }

  return m_childStates[idx];
}

void cs::SpatialState::AddSpatialState(cs::SpatialState *state)
{
  if (state)
  {
    state->AddRef();
    state->m_parentState = this;
    m_childStates.push_back(state);
    FlagBoundingBoxDirty();
  }
}

void cs::SpatialState::RemoveSpatialState(cs::SpatialState *state)
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

cs::SpatialState *cs::SpatialState::FindState(const std::string &stateName)
{
  if (stateName == GetName())
  {
    return this;
  }

  for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
  {
    cs::SpatialState *state = m_childStates[i]->FindState(stateName);
    if (state)
    {
      return state;
    }
  }
  return 0;
}

const cs::SpatialState *cs::SpatialState::FindState(const std::string &stateName) const
{
  if (stateName == GetName())
  {
    return this;
  }

  for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
  {
    cs::SpatialState *state = m_childStates[i]->FindState(stateName);
    if (state)
    {
      return state;
    }
  }
  return 0;
}


void cs::SpatialState::Scan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
{
  if (clipper)
  {
    cs::Clipper::ClipResult cr = clipper->Test(m_boundingBox);
    switch (cr)
    {
    case cs::Clipper::eCR_In:
      // we are completely within the viewing frustum no need to test states below.
      clipper = 0;
      break;

    case cs::Clipper::eCR_Out:
      // we are out... nothin to clip
      return;

    case cs::Clipper::eCR_Intermediate:
      // we are partially within the viewing frustum.. the entity will be rendered, but states below 
      // must be tested aswell.
      break;
    }
  }

  cs::Vector3f dir;
  cs::Vector3f::Sub(config.MainCameraPosition, m_boundingBox.GetCenter(), dir);
  float distanceSqr = dir.Dot();

  m_distanceState.UpdateDistance(distanceSqr);
  if (m_distanceState.IsOut())
  {
    return;
  }


  PrivScan(clipper, graphics, entityScan, config);

  for (csSize i = 0, in = m_childStates.size(); i < in; ++i)
  {
    m_childStates[i]->Scan(clipper, graphics, entityScan, config);
  }
}

void cs::SpatialState::PrivScan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
{

}





#define CS_NUM_FADE_VALUES 18
#define CS_FADE_VALUE_OUT 0
#define CS_FADE_VALUE_FIRST_IN 1
#define CS_FADE_VALUE_IN 17
#define CS_FADE_VALUE_FIRST_OUT 16


cs::DistanceState::DistanceState()
  : m_min (-FLT_MAX)
  , m_max (FLT_MAX)
  , m_state(eInitial)
{
  SetFadeTime(500L);
}

void cs::DistanceState::SetupDistance(float min, float max)
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

void cs::DistanceState::SetupState(State state, csUInt8 fadeValue)
{
  m_fadeValue = fadeValue;
  m_state = state;
}

void cs::DistanceState::SetFadeTime(csUInt64 fadeTimeMilli)
{
  m_fadeTimeMicroPerValue = fadeTimeMilli * 1000L / (csUInt64)CS_NUM_FADE_VALUES;
}

void cs::DistanceState::UpdateDistance(float distance)
{
  bool in = m_min <= distance && distance <= m_max;
  csUInt64 ct = cs::Time::Get().GetCurrentTimeMicro();

  switch (m_state)
  {
  case eInitial:
    if (in)
    {
      m_state = eIn;
      m_fadeValue = CS_FADE_VALUE_IN;
    }
    else
    {
      m_state = eOut;
      m_fadeValue = CS_FADE_VALUE_OUT;
    }
    break;


  case eOut:
    if (in)
    {
      m_state = eFadeIn;
      m_nextFadeValueChange = ct + m_fadeTimeMicroPerValue;
      m_fadeValue = CS_FADE_VALUE_FIRST_IN;
    }
    break;
  case eFadeIn:
    if (in)
    {
      if (ct >= m_nextFadeValueChange)
      {
        m_fadeValue++;
        if (m_fadeValue == CS_FADE_VALUE_IN)
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
      m_nextFadeValueChange = cs::Time::Get().GetCurrentTimeMicro() + m_fadeTimeMicroPerValue;
      m_fadeValue = CS_FADE_VALUE_FIRST_OUT;
    }
    break;
  case eFadeOut:
    if (!in)
    {
      csUInt64 ct = cs::Time::Get().GetCurrentTimeMicro();
      if (ct >= m_nextFadeValueChange)
      {
        m_fadeValue--;
        if (m_fadeValue == CS_FADE_VALUE_OUT)
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
