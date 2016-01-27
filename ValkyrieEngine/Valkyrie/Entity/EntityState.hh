#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Entity/EntityState.refl.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Types.hh>

class vkClipper;
class vkEntity;
class vkEntityScene;
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

  virtual void SetID(vkID id);
  vkID GetID() const;

  void SetName(const vkString &name);
  const vkString &GetName() const;

  virtual vkSpatialState *ToSpatialState();
  virtual const vkSpatialState *ToSpatialState() const;

  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);
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

VK_FORCEINLINE void vkEntityState::SetID(vkID id)
{
  m_id = id;
}

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

