#pragma once


struct vkMatrix4f;
class vkMesh;
class vkSubMesh;
class vkPhysicsShapeContainer;
struct vkPhysGeometry;
struct IPhysicsShape;

namespace mesh
{



class CollisionToMeshConverter
{

public:
  static vkMesh *CreateMesh(vkPhysicsShapeContainer *shapes);
  static vkMesh *CreateMesh(IPhysicsShape *shape);

private:
  CollisionToMeshConverter() { }

  static vkSubMesh *CreateSubMesh(IPhysicsShape *shape);
  static vkSubMesh *CreateCylinderZSubMesh(const vkPhysGeometry &geometry, const vkMatrix4f &localTransform);

};

}