#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Entity/EntityState.refl.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Types.hh>

class vkClipper;
class vkEntity;
class vkSpatialState;
struct IEntityScan;
struct IGraphics;

/**
* \ingroup entity
*/

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

  virtual void OnAttachedToEntity(vkEntity *entity);
  virtual void OnDetachedFromEntity(vkEntity *entity);

  vkEntity *GetEntity();
  const vkEntity *GetEntity() const;

protected:
  vkEntityState();



private:
  vkID m_id;
  vkString m_name;

  vkEntity *m_entity;
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

VK_FORCEINLINE vkEntity *vkEntityState::GetEntity()
{
  return m_entity;
}

VK_FORCEINLINE const vkEntity *vkEntityState::GetEntity() const
{
  return m_entity;
}

