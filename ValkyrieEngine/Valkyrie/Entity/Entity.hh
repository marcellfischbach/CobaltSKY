#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Defs.hh>
#include <vector>
#include <Valkyrie/Entity/Entity.refl.hh>

class vkClipper;
class vkEntityState;
class vkSpatialState;
class vkTransformation;

struct vkScanConfig;
struct IEntityScan;
struct IGraphics;

VK_CLASS()
class VKE_API vkEntity : public vkObject
{
  VK_CLASS_GEN;
public:
  vkEntity();
  virtual ~vkEntity();

  vkID GetID() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  void SetRootState(vkSpatialState *rootState);
  void AddState(vkEntityState *state);
  void AddState(vkSpatialState *state, vkSpatialState *parentState);

  vkTransformation GetTransformation();
  void FinishTransformation();

  static vkID GetNextID();

  virtual void Scan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

private:
  vkID m_id;
  vkString m_name;

  vkSpatialState *m_rootState;

  std::vector<vkEntityState*> m_states;
};

VK_FORCEINLINE vkID vkEntity::GetID() const
{
  return m_id;
}



VK_FORCEINLINE void vkEntity::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE const vkString &vkEntity::GetName() const
{
  return m_name;
}

