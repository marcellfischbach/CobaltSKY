#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <bullet/btBulletCollisionCommon.h>
#include <physicsbullet/bulletshape.refl.hh>

CS_CLASS()
class CSBULLETPHYSICS_API csBulletShape : public CS_SUPER(iPhysicsShape)
{
  CS_CLASS_GEN_OBJECT;
public:
  csBulletShape();
  virtual ~csBulletShape();

  virtual const csPhysGeometry &GetGeometry() const;
  virtual void SetLocalTransform(const csMatrix4f &localTransform);
  virtual const csMatrix4f &GetLocalTransform() const;

  bool IsTransformed() const;

public:
  virtual bool Initialize(const csPhysGeometry &geometry);

  btCollisionShape *GetBulletShape();

private:
  csMatrix4f m_localTransform;
  csPhysGeometry m_geometry;
  btCollisionShape *m_bulletShape;
};

CS_FORCEINLINE btCollisionShape *csBulletShape::GetBulletShape()
{
  return m_bulletShape;
}

