#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/physics/iphysicsshape.refl.hh>

namespace cs
{

struct PhysGeometryHeightFieldData
{
  cs::Vector3f Min;
  cs::Vector3f Max;
  csUInt16 SideLength;
  csUInt16* Heights;
};

struct PhysGeometryTriMeshData
{
  csUInt16 NumVertices;
  cs::Vector3f* Vertices;
  csUInt16 NumIndices;
  csUInt16* Indices;
};

struct PhysGeometry
{
  cs::ePhysGeometryType Type;

  float Radius;
  float Height;
  cs::Vector3f Dimensions;

  cs::PhysGeometryHeightFieldData* HeightFieldData;
  cs::PhysGeometryTriMeshData* TriMeshData;
};


CS_CLASS()
struct CSE_API iPhysicsShape : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsShape() { }

  virtual const cs::PhysGeometry& GetGeometry() const = 0;

  virtual void SetLocalTransform(const cs::Matrix4f& localTransform) = 0;
  virtual const cs::Matrix4f& GetLocalTransform() const = 0;
};


}

