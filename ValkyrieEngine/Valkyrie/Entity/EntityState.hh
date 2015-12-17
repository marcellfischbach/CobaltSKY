#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Entity/EntityState.refl.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Types.hh>

class vkClipper;
class vkSpatialState;
struct IEntityScan;
struct IGraphics;

VK_INTERFACE()
class VKE_API vkEntityState : public vkObject
{
  VK_CLASS_GEN;

public:
  virtual ~vkEntityState();


  vkID GetID() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  virtual vkSpatialState *ToSpatialState();
  virtual const vkSpatialState *ToSpatialState() const;

protected:
  vkEntityState();


private:
  vkID m_id;
  vkString m_name;
};


VK_FORCEINLINE vkID vkEntityState::GetID() const
{
  return m_id;
}



VK_FORCEINLINE void vkEntityState::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE const vkString &vkEntityState::GetName() const
{
  return m_name;
}

