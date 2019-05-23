#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/math/csboundingbox.hh>
#include <vector>
#include <cobalt/entity/csspatialstate.refl.hh>
#include <float.h>


namespace cs
{
struct ScanConfig;

class CSE_API DistanceState
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
  DistanceState();

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
class CSE_API SpatialState : public CS_SUPER(cs::EntityState)
{
  friend class cs::Entity;
  CS_CLASS_GEN;

public:
  SpatialState();
  virtual ~SpatialState();

  cs::Transformation GetTransformation();
  virtual void FinishTransformation();

  CS_FUNCTION()
    void SetLocalMatrix(const cs::Matrix4f & matrix);
  CS_FUNCTION()
    const cs::Matrix4f& GetLocalMatrix() const;

  const cs::Matrix4f& GetLocalTransformation() const;
  const cs::Matrix4f& GetGlobalTransformation() const;
  const cs::Matrix4f& GetGlobalTransformationInv() const;
  const cs::BoundingBox& GetBoundingBox() const;

  virtual cs::SpatialState* ToSpatialState();
  virtual const cs::SpatialState* ToSpatialState() const;

  cs::SpatialState* FindState(const std::string & stateName);
  const cs::SpatialState* FindState(const std::string & stateName) const;

  csSize GetNumberOfChildState() const;
  cs::SpatialState* GetChildState(csSize idx);
  const cs::SpatialState* GetChildState(csSize idx) const;


  cs::SpatialState* GetParentState();
  const cs::SpatialState* GetParentState() const;

  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

  void UpdateBoundingBox();

  virtual void Scan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);

protected:
  virtual void UpdateTransformation();
  virtual void FillBoundingBox(cs::BoundingBox & bbox);
  virtual void PrivScan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);

  void PerformTransformation();

  CS_PROPERTY()
    cs::Matrix4f m_localMatrix;
  CS_PROPERTY()
    cs::Matrix4f m_globalMatrix;
  CS_PROPERTY()
    cs::Matrix4f m_globalMatrixInv;

  bool m_boundingBoxDirty;
  cs::BoundingBox m_boundingBox;
  cs::DistanceState m_distanceState;

private:
  void FlagBoundingBoxDirty();
  void FlagParentBoundingBoxDirty();
  void AddSpatialState(cs::SpatialState * state);
  void RemoveSpatialState(cs::SpatialState * state);



  cs::SpatialState * m_parentState;
  std::vector<cs::SpatialState*> m_childStates;

};

}

CS_FORCEINLINE const cs::Matrix4f &cs::SpatialState::GetLocalTransformation() const
{
  return m_localMatrix;
}

CS_FORCEINLINE const cs::Matrix4f &cs::SpatialState::GetGlobalTransformation() const
{
  return m_globalMatrix;
}


CS_FORCEINLINE const cs::Matrix4f &cs::SpatialState::GetGlobalTransformationInv() const
{
  return m_globalMatrixInv;
}

CS_FORCEINLINE const cs::BoundingBox &cs::SpatialState::GetBoundingBox() const
{
  return m_boundingBox;
}

CS_FORCEINLINE cs::SpatialState *cs::SpatialState::GetParentState()
{
  return m_parentState;
}

CS_FORCEINLINE const cs::SpatialState *cs::SpatialState::GetParentState() const
{
  return m_parentState;
}




CS_FORCEINLINE csUInt8 cs::DistanceState::GetFadeValue() const
{
  return m_fadeValue;
}

CS_FORCEINLINE bool cs::DistanceState::IsOut() const
{
  return m_state == eOut;
}
