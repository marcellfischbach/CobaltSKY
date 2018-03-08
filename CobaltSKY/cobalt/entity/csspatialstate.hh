#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/math/csboundingbox.hh>
#include <vector>
#include <cobalt/entity/csspatialstate.refl.hh>
#include <float.h>

struct csScanConfig;

class CSE_API csDistanceState
{
public:
  enum State
  {
    eInitial,
    eIn,
    eOut,
    eFadeIn,
    eFadeOut,
  };

public:
  csDistanceState();

  void SetupDistance(float min = -FLT_MAX, float max = FLT_MAX);
  void SetupState(State state, csUInt8 fadeValue);
  void SetFadeTime(csUInt64 timeMilli);

  void UpdateDistance(float distanceSqr);

  csUInt8 GetFadeValue() const;

  bool IsOut() const;

private:
  csUInt64 m_fadeTimeMicroPerValue;
  float m_min;
  float m_max;


  csUInt8 m_fadeValue;
  State m_state;
  csUInt64 m_nextFadeValueChange;
};

/**
* \ingroup entity
*/
CS_CLASS()
class CSE_API csSpatialState : public CS_SUPER(csEntityState)
{
  friend class csEntity;
  CS_CLASS_GEN;

public:
  csSpatialState();
  virtual ~csSpatialState();

  csTransformation GetTransformation();
  virtual void FinishTransformation();

  CS_FUNCTION()
    void SetLocalMatrix(const csMatrix4f &matrix);
  CS_FUNCTION()
    const csMatrix4f &GetLocalMatrix() const;

  const csMatrix4f &GetLocalTransformation() const;
  const csMatrix4f &GetGlobalTransformation() const;
  const csMatrix4f &GetGlobalTransformationInv() const;
  const csBoundingBox &GetBoundingBox() const;

  virtual csSpatialState *ToSpatialState();
  virtual const csSpatialState *ToSpatialState() const;

  csSpatialState *FindState(const std::string &stateName);
  const csSpatialState *FindState(const std::string &stateName) const;

  csSize GetNumberOfChildState() const;
  csSpatialState *GetChildState(csSize idx);
  const csSpatialState *GetChildState(csSize idx) const;


  csSpatialState *GetParentState();
  const csSpatialState *GetParentState() const;

  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

  void UpdateBoundingBox();

  virtual void Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);

protected:
  virtual void UpdateTransformation();
  virtual void FillBoundingBox (csBoundingBox &bbox);
  virtual void PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);

  void PerformTransformation();

  CS_PROPERTY()
    csMatrix4f m_localMatrix;
  CS_PROPERTY()
    csMatrix4f m_globalMatrix;
  CS_PROPERTY()
    csMatrix4f m_globalMatrixInv;

  bool m_boundingBoxDirty;
  csBoundingBox m_boundingBox;
  csDistanceState m_distanceState;

private:
  void FlagBoundingBoxDirty();
  void FlagParentBoundingBoxDirty();
  void AddSpatialState(csSpatialState *state);
  void RemoveSpatialState(csSpatialState *state);


  
  csSpatialState *m_parentState;
  std::vector<csSpatialState*> m_childStates;

};


CS_FORCEINLINE const csMatrix4f &csSpatialState::GetLocalTransformation() const
{
  return m_localMatrix;
}

CS_FORCEINLINE const csMatrix4f &csSpatialState::GetGlobalTransformation() const
{
  return m_globalMatrix;
}


CS_FORCEINLINE const csMatrix4f &csSpatialState::GetGlobalTransformationInv() const
{
  return m_globalMatrixInv;
}

CS_FORCEINLINE const csBoundingBox &csSpatialState::GetBoundingBox() const
{
  return m_boundingBox;
}

CS_FORCEINLINE csSpatialState *csSpatialState::GetParentState()
{
  return m_parentState;
}

CS_FORCEINLINE const csSpatialState *csSpatialState::GetParentState() const
{
  return m_parentState;
}




CS_FORCEINLINE csUInt8 csDistanceState::GetFadeValue() const
{
  return m_fadeValue;
}

CS_FORCEINLINE bool csDistanceState::IsOut() const
{
  return m_state == eOut;
}
