#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/physics/iphysicsshape.refl.hh>


struct csPhysGeometryHeightFieldData
{
  csVector3f Min;
  csVector3f Max;
  csUInt16 SideLength;
  csUInt16 *Heights;
};

struct csPhysGeometryTriMeshData
{
  csUInt16 NumVertices;
  csVector3f *Vertices;
  csUInt16 NumIndices;
  csUInt16 *Indices;
};

struct csPhysGeometry
{
  csPhysGeometryType Type;

  float Radius;
  float Height;
  csVector3f Dimensions;

  csPhysGeometryHeightFieldData *HeightFieldData;
  csPhysGeometryTriMeshData *TriMeshData;
};


CS_CLASS()
struct CSE_API iPhysicsShape : public cs::iObject
{
  CS_CLASS_GEN;
  virtual ~iPhysicsShape () { }

  virtual const csPhysGeometry &GetGeometry() const = 0;

  virtual void SetLocalTransform(const csMatrix4f &localTransform) = 0;
  virtual const csMatrix4f &GetLocalTransform() const = 0;
};


