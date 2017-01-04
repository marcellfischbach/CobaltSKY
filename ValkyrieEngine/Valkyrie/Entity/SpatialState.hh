#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.refl.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <vector>
#include <Valkyrie/Entity/SpatialState.refl.hh>

struct vkScanConfig;

class VKE_API vkDistanceState
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
  vkDistanceState();

  void SetupDistance(float min = -FLT_MAX, float max = FLT_MAX);
  void SetupState(State state, vkUInt8 fadeValue);
  void SetFadeTime(vkUInt64 timeMilli);

  void UpdateDistance(float distanceSqr);

  vkUInt8 GetFadeValue() const;

  bool IsOut() const;

private:
  vkUInt64 m_fadeTimeMicroPerValue;
  float m_min;
  float m_max;


  vkUInt8 m_fadeValue;
  State m_state;
  vkUInt64 m_nextFadeValueChange;
};

/**
* \ingroup entity
*/
VK_CLASS()
class VKE_API vkSpatialState : public VK_SUPER(vkEntityState)
{
  friend class vkEntity;
  VK_CLASS_GEN;

public:
  vkSpatialState();
  virtual ~vkSpatialState();

  vkTransformation GetTransformation();
  virtual void FinishTransformation();

  const vkMatrix4f &GetLocalTransformation() const;
  const vkMatrix4f &GetGlobalTransformation() const;
  const vkMatrix4f &GetGlobalTransformationInv() const;
  const vkBoundingBox &GetBoundingBox() const;

  virtual vkSpatialState *ToSpatialState();
  virtual const vkSpatialState *ToSpatialState() const;

  vkSpatialState *FindState(const vkString &stateName);
  const vkSpatialState *FindState(const vkString &stateName) const;

  vkSize GetNumberOfChildState() const;
  vkSpatialState *GetChildState(vkSize idx);
  const vkSpatialState *GetChildState(vkSize idx) const;


  vkSpatialState *GetParentState();
  const vkSpatialState *GetParentState() const;

  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

  void UpdateBoundingBox();

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

protected:
  virtual void UpdateTransformation();
  virtual void FillBoundingBox (vkBoundingBox &bbox);
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

  void PerformTransformation();

  vkMatrix4f m_localMatrix;
  vkMatrix4f m_globalMatrix;
  vkMatrix4f m_globalMatrixInv;

  bool m_boundingBoxDirty;
  vkBoundingBox m_boundingBox;
  vkDistanceState m_distanceState;


private:
  void FlagBoundingBoxDirty();
  void FlagParentBoundingBoxDirty();
  void AddSpatialState(vkSpatialState *state);
  void RemoveSpatialState(vkSpatialState *state);


  
  vkSpatialState *m_parentState;
  std::vector<vkSpatialState*> m_childStates;

};



VK_FORCEINLINE const vkMatrix4f &vkSpatialState::GetLocalTransformation() const
{
  return m_localMatrix;
}

VK_FORCEINLINE const vkMatrix4f &vkSpatialState::GetGlobalTransformation() const
{
  return m_globalMatrix;
}


VK_FORCEINLINE const vkMatrix4f &vkSpatialState::GetGlobalTransformationInv() const
{
  return m_globalMatrixInv;
}

VK_FORCEINLINE const vkBoundingBox &vkSpatialState::GetBoundingBox() const
{
  return m_boundingBox;
}

VK_FORCEINLINE vkSpatialState *vkSpatialState::GetParentState()
{
  return m_parentState;
}

VK_FORCEINLINE const vkSpatialState *vkSpatialState::GetParentState() const
{
  return m_parentState;
}




VK_FORCEINLINE vkUInt8 vkDistanceState::GetFadeValue() const
{
  return m_fadeValue;
}

VK_FORCEINLINE bool vkDistanceState::IsOut() const
{
  return m_state == eOut;
}
