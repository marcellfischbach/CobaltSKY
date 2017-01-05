#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/physics/iphysicsshape.hh>
#include <bullet/btBulletCollisionCommon.h>
#include <PhysicsBullet/BulletShape.refl.hh>

VK_CLASS()
class VKBULLET_API vkBulletShape : public VK_SUPER(IPhysicsShape)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkBulletShape();
  virtual ~vkBulletShape();

  virtual const vkPhysGeometry &GetGeometry() const;
  virtual void SetLocalTransform(const vkMatrix4f &localTransform);
  virtual const vkMatrix4f &GetLocalTransform() const;

  bool IsTransformed() const;

public:
  virtual bool Initialize(const vkPhysGeometry &geometry);

  btCollisionShape *GetBulletShape();

private:
  vkMatrix4f m_localTransform;
  vkPhysGeometry m_geometry;
  btCollisionShape *m_bulletShape;
};

VK_FORCEINLINE btCollisionShape *vkBulletShape::GetBulletShape()
{
  return m_bulletShape;
}

