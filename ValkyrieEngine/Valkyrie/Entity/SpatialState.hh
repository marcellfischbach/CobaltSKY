#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/EntityState.hh>
#include <Valkyrie/Entity/SpatialState.refl.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <vector>


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


  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);

protected:
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan);


  vkMatrix4f m_localMatrix;
  vkMatrix4f m_globalMatrix;

private:
  void AddSpatialState(vkSpatialState *state);
  void RemoveSpatialState(vkSpatialState *state);


  vkSpatialState *m_parentState;
  std::vector<vkSpatialState*> m_childStates;
};