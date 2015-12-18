#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.refl.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <vector>

struct vkScanConfig;

/**
* \ingroup entity
*/
VK_CLASS()
class VKE_API vkSpatialState : public vkEntityState
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
  const vkBoundingBox &GetBoundingBox() const;

  virtual vkSpatialState *ToSpatialState();
  virtual const vkSpatialState *ToSpatialState() const;

  vkSpatialState *FindState(const vkString &stateName);
  const vkSpatialState *FindState(const vkString &stateName) const;

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

protected:
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);


  vkMatrix4f m_localMatrix;
  vkMatrix4f m_globalMatrix;

  vkBoundingBox m_boundingBox;

private:
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

VK_FORCEINLINE const vkBoundingBox &vkSpatialState::GetBoundingBox() const
{
  return m_boundingBox;
}
