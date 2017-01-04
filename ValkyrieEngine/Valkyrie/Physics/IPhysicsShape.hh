#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Physics/IPhysicsShape.refl.hh>


struct vkPhysGeometryHeightFieldData
{
  vkVector3f Min;
  vkVector3f Max;
  vkUInt16 SideLength;
  vkUInt16 *Heights;
};

struct vkPhysGeometryTriMeshData
{
  vkUInt16 NumVertices;
  vkVector3f *Vertices;
  vkUInt16 NumIndices;
  vkUInt16 *Indices;
};

struct vkPhysGeometry
{
  vkPhysGeometryType Type;

  float Radius;
  float Height;
  vkVector3f Dimensions;

  vkPhysGeometryHeightFieldData *HeightFieldData;
  vkPhysGeometryTriMeshData *TriMeshData;
};


VK_INTERFACE()
struct VKE_API IPhysicsShape : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsShape () { }

  virtual const vkPhysGeometry &GetGeometry() const = 0;

  virtual void SetLocalTransform(const vkMatrix4f &localTransform) = 0;
  virtual const vkMatrix4f &GetLocalTransform() const = 0;
};


