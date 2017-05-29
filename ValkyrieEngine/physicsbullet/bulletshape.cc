

#include <physicsbullet/bulletshape.hh>

vkBulletShape::vkBulletShape()
  : iPhysicsShape()
{
  VK_CLASS_GEN_CONSTR;
}

vkBulletShape::~vkBulletShape()
{

}


const vkPhysGeometry &vkBulletShape::GetGeometry() const
{
  return m_geometry;
}

void vkBulletShape::SetLocalTransform(const vkMatrix4f &localTransform)
{
  m_localTransform = localTransform;
}

const vkMatrix4f &vkBulletShape::GetLocalTransform() const
{
  return m_localTransform;
}

bool vkBulletShape::IsTransformed() const
{
  return !m_localTransform.IsIdentity();
}

bool vkBulletShape::Initialize(const vkPhysGeometry &geometry)
{
  memcpy(&m_geometry, &geometry, sizeof(vkPhysGeometry));

  /*
  printf("Initialize: \n");
  printf("   Type:   %d\n", geometry.Type);
  printf("   Size:   %f %f %f\n", geometry.Dimensions.x, geometry.Dimensions.y, geometry.Dimensions.z);
  printf("   Radius: %f\n", geometry.Radius);
  printf("   Height: %f\n", geometry.Height);
  */
  switch (geometry.Type)
  {
  case ePGT_Sphere:
    m_bulletShape = new btSphereShape(geometry.Radius);
    break;
  case ePGT_CylinderX:
    m_bulletShape = new btCylinderShapeX(btVector3(geometry.Height * 0.5f, geometry.Radius, geometry.Radius));
    break;
  case ePGT_CylinderY:
    m_bulletShape = new btCylinderShape(btVector3(geometry.Radius, geometry.Height * 0.5f, geometry.Radius));
    break;
  case ePGT_CylinderZ:
    m_bulletShape = new btCylinderShapeZ(btVector3(geometry.Radius, geometry.Radius, geometry.Height * 0.5f));
    break;
  case ePGT_CapsuleX:
    m_bulletShape = new btCapsuleShapeX(geometry.Radius, geometry.Height);
    break;
  case ePGT_CapsuleY:
    m_bulletShape = new btCapsuleShape(geometry.Radius, geometry.Height);
    break;
  case ePGT_CapsuleZ:
    m_bulletShape = new btCapsuleShapeZ(geometry.Radius, geometry.Height);
    break;
  case ePGT_Box:
    m_bulletShape = new btBoxShape(btVector3(geometry.Dimensions.x * 0.5f, geometry.Dimensions.y * 0.5f, geometry.Dimensions.z * 0.5f));
    break;

  default:
    printf("Shape type %d currently not implemented\n", geometry.Type);
    return false;
  }

  return true;
}


