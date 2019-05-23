#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <bullet/btBulletCollisionCommon.h>
#include <physicsbullet/bulletshape.refl.hh>

namespace cs
{

CS_CLASS()
class CSBULLETPHYSICS_API BulletShape : public CS_SUPER(cs::iPhysicsShape)
{
  CS_CLASS_GEN_OBJECT;
public:
  BulletShape();
  virtual ~BulletShape();

  virtual const cs::PhysGeometry& GetGeometry() const;
  virtual void SetLocalTransform(const cs::Matrix4f & localTransform);
  virtual const cs::Matrix4f& GetLocalTransform() const;

  bool IsTransformed() const;

public:
  virtual bool Initialize(const cs::PhysGeometry & geometry);

  btCollisionShape* GetBulletShape();

private:
  cs::Matrix4f m_localTransform;
  cs::PhysGeometry m_geometry;
  btCollisionShape* m_bulletShape;
};

}

CS_FORCEINLINE btCollisionShape *cs::BulletShape::GetBulletShape()
{
  return m_bulletShape;
}

