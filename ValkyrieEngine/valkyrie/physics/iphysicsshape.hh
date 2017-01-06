#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/physics/iphysicsshape.refl.hh>


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


