#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <bullet/btBulletCollisionCommon.h>
#include <physicsbullet/bulletshape.refl.hh>

CS_CLASS()
class CSBULLETPHYSICS_API csBulletShape : public CS_SUPER(cs::iPhysicsShape)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletShape();
  virtual ~csBulletShape();

  virtual const cs::PhysGeometry &GetGeometry() const;
  virtual void SetLocalTransform(const cs::Matrix4f &localTransform);
  virtual const cs::Matrix4f &GetLocalTransform() const;

  bool IsTransformed() const;

public:
  virtual bool Initialize(const cs::PhysGeometry &geometry);

  btCollisionShape *GetBulletShape();

private:
  cs::Matrix4f m_localTransform;
  cs::PhysGeometry m_geometry;
  btCollisionShape *m_bulletShape;
};

CS_FORCEINLINE btCollisionShape *csBulletShape::GetBulletShape()
{
  return m_bulletShape;
}

