

#include <physicsbullet/bulletshape.hh>

cs::BulletShape::BulletShape()
  : cs::iPhysicsShape()
{
  CS_CLASS_GEN_CONSTR;
}

cs::BulletShape::~BulletShape()
{

}


const cs::PhysGeometry &cs::BulletShape::GetGeometry() const
{
  return m_geometry;
}

void cs::BulletShape::SetLocalTransform(const cs::Matrix4f &localTransform)
{
  m_localTransform = localTransform;
}

const cs::Matrix4f &cs::BulletShape::GetLocalTransform() const
{
  return m_localTransform;
}

bool cs::BulletShape::IsTransformed() const
{
  return !m_localTransform.IsIdentity();
}

bool cs::BulletShape::Initialize(const cs::PhysGeometry &geometry)
{
  memcpy(&m_geometry, &geometry, sizeof(cs::PhysGeometry));

  /*
  printf("Initialize: \n");
  printf("   Type:   %d\n", geometry.Type);
  printf("   Size:   %f %f %f\n", geometry.Dimensions.x, geometry.Dimensions.y, geometry.Dimensions.z);
  printf("   Radius: %f\n", geometry.Radius);
  printf("   Height: %f\n", geometry.Height);
  */
  switch (geometry.Type)
  {
  case cs::ePGT_Sphere:
    m_bulletShape = new btSphereShape(geometry.Radius);
    break;
  case cs::ePGT_CylinderX:
    m_bulletShape = new btCylinderShapeX(btVector3(geometry.Height * 0.5f, geometry.Radius, geometry.Radius));
    break;
  case cs::ePGT_CylinderY:
    m_bulletShape = new btCylinderShape(btVector3(geometry.Radius, geometry.Height * 0.5f, geometry.Radius));
    break;
  case cs::ePGT_CylinderZ:
    m_bulletShape = new btCylinderShapeZ(btVector3(geometry.Radius, geometry.Radius, geometry.Height * 0.5f));
    break;
  case cs::ePGT_CapsuleX:
    m_bulletShape = new btCapsuleShapeX(geometry.Radius, geometry.Height);
    break;
  case cs::ePGT_CapsuleY:
    m_bulletShape = new btCapsuleShape(geometry.Radius, geometry.Height);
    break;
  case cs::ePGT_CapsuleZ:
    m_bulletShape = new btCapsuleShapeZ(geometry.Radius, geometry.Height);
    break;
  case cs::ePGT_Box:
    m_bulletShape = new btBoxShape(btVector3(geometry.Dimensions.x * 0.5f, geometry.Dimensions.y * 0.5f, geometry.Dimensions.z * 0.5f));
    break;

  default:
    printf("Shape type %d currently not implemented\n", geometry.Type);
    return false;
  }

  return true;
}


