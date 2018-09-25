
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/math/csclipper.hh>
#include <cobalt/cstime.hh>
#include <cobalt/entity/cscolliderstate.hh>

csSpatialState::csSpatialState()
  : csEntityState()
  , m_parentState(0)
  , m_boundingBoxDirty(true)
{

}

csSpatialState::~csSpatialState()
{

}

csSpatialState *csSpatialState::ToSpatialState()
{
  return this;
}

const csSpatialState *csSpatialState::ToSpatialState() const
{
  return this;
}



csTransformation csSpatialState::GetTransformation()
{
  if (m_parentState)
  {
    return csTransformation(&m_localMatrix, &m_globalMatrix, &m_parentState->m_globalMatrix, &m_parentState->m_globalMatrixInv);
  }
  else
  {
    return csTransformation(&m_localMatrix, &m_globalMatrix, 0, 0);
  }
}

void csSpatialState::FinishTransformation()
{
  PerformTransformation();
}

void csSpatialState::SetLocalMatrix(const csMatrix4f &localMatrix)
{
  m_localMatrix.Set(localMatrix);
}

const csMatrix4f &csSpatialState::GetLocalMatrix() const
{
  return m_localMatrix;
}

void csSpatialState::PerformTransformation()
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

void csSpatialState::UpdateTransformation()
{
  if (m_parentState)
  {
    csMatrix4f::Mult(m_parentState->GetGlobalTransformation(), m_localMatrix, m_globalMatrix);
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

void csSpatialState::FlagParentBoundingBoxDirty()
{
  if (m_parentState)
  {
    m_parentState->FlagBoundingBoxDirty();
  }
}

void csSpatialState::FlagBoundingBoxDirty()
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

void csSpatialState::SetClippingRange(float min, float max)
{
  m_distanceState.SetupDistance(min, max);

}

void csSpatialState::UpdateBoundingBox()
{
  if (!m_boundingBoxDirty)
  {
    return;
  }
  m_boundingBox.Clear();

  FillBoundingBox(m_boundingBox);

  m_boundingBox.Finish();
}

void csSpatialState::FillBoundingBox(csBoundingBox &bbox)
{
  for (csSize i = 0, in = GetNumberOfChildState(); i < in; ++i)
  {
    csSpatialState *state = GetChildState(i);
    if (state)
    {
      state->UpdateBoundingBox();
      bbox.Add(state->GetBoundingBox());
    }
  }
}

csSize csSpatialState::GetNumberOfChildState() const
{
  return m_childStates.size();
}

csSpatialState *csSpatialState::GetChildState(csSize idx)
{
  if (idx >= m_childStates.size())
  {
    return 0;
  }

  return m_childStates[idx];
}

const csSpatialState *csSpatialState::GetChildState(csSize idx) const
{
  if (idx >= m_childStates.size())
  {
    return 0;
  }

  return m_childStates[idx];
}

void csSpatialState::AddSpatialState(csSpatialState *state)
{
  if (state)
  {
    state->AddRef();
    state->m_parentState = this;
    m_childStates.push_back(state);
    FlagBoundingBoxDirty();
  }
}

void csSpatialState::RemoveSpatialState(csSpatialState *state)
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

csSpatialState *csSpatialState::FindState(const std::string &stateName)
{
  if (stateName == GetName())
  {
    return this;
  }

  for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
  {
    csSpatialState *state = m_childStates[i]->FindState(stateName);
    if (state)
    {
      return state;
    }
  }
  return 0;
}

const csSpatialState *csSpatialState::FindState(const std::string &stateName) const
{
  if (stateName == GetName())
  {
    return this;
  }

  for (size_t i = 0, in = m_childStates.size(); i < in; ++i)
  {
    csSpatialState *state = m_childStates[i]->FindState(stateName);
    if (state)
    {
      return state;
    }
  }
  return 0;
}


void csSpatialState::Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  if (clipper)
  {
    csClipper::ClipResult cr = clipper->Test(m_boundingBox);
    switch (cr)
    {
    case csClipper::eCR_In:
      // we are completely within the viewing frustum no need to test states below.
      clipper = 0;
      break;

    case csClipper::eCR_Out:
      // we are out... nothin to clip
      return;

    case csClipper::eCR_Intermediate:
      // we are partially within the viewing frustum.. the entity will be rendered, but states below 
      // must be tested aswell.
      break;
    }
  }

  csVector3f dir;
  csVector3f::Sub(config.MainCameraPosition, m_boundingBox.GetCenter(), dir);
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

void csSpatialState::PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{

}





#define CS_NUM_FADE_VALUES 18
#define CS_FADE_VALUE_OUT 0
#define CS_FADE_VALUE_FIRST_IN 1
#define CS_FADE_VALUE_IN 17
#define CS_FADE_VALUE_FIRST_OUT 16


csDistanceState::csDistanceState()
  : m_min (-FLT_MAX)
  , m_max (FLT_MAX)
  , m_state(eInitial)
{
  SetFadeTime(500L);
}

void csDistanceState::SetupDistance(float min, float max)
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

void csDistanceState::SetupState(State state, csUInt8 fadeValue)
{
  m_fadeValue = fadeValue;
  m_state = state;
}

void csDistanceState::SetFadeTime(csUInt64 fadeTimeMilli)
{
  m_fadeTimeMicroPerValue = fadeTimeMilli * 1000L / (csUInt64)CS_NUM_FADE_VALUES;
}

void csDistanceState::UpdateDistance(float distance)
{
  bool in = m_min <= distance && distance <= m_max;
  csUInt64 ct = csTime::Get().GetCurrentTimeMicro();

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
      m_nextFadeValueChange = csTime::Get().GetCurrentTimeMicro() + m_fadeTimeMicroPerValue;
      m_fadeValue = CS_FADE_VALUE_FIRST_OUT;
    }
    break;
  case eFadeOut:
    if (!in)
    {
      csUInt64 ct = csTime::Get().GetCurrentTimeMicro();
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
