#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/graphics/cssubmesh.refl.hh>




namespace cs
{
struct iGraphics;
struct iIndexBuffer;
struct iVertexBuffer;
struct iVertexDeclaration;

CS_CLASS()
class CSE_API SubMesh : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  SubMesh();
  virtual ~SubMesh();

  virtual void Render(cs::iGraphics * renderer);

  void SetPrimitiveType(cs::ePrimitiveType type);
  void SetIndexType(cs::eDataType indexType);
  void SetVertexDeclaration(cs::iVertexDeclaration * vertexDeclaration);
  void AddVertexBuffer(cs::iVertexBuffer * vertexBuffer);
  void SetIndexBuffer(cs::iIndexBuffer * indexBuffer, csSize count, csSize offset = 0);

  void SetBoundingBox(const cs::BoundingBox & bbox);
  const cs::BoundingBox & GetBoundingBox() const;

  unsigned GetNumberOfTrigons() const;

private:

  cs::iIndexBuffer * m_indexBuffer;
  csSize        m_offset;
  csSize        m_count;

  cs::iVertexDeclaration * m_vertexDeclaration;
  std::vector<cs::iVertexBuffer*> m_vertexBuffer;

  cs::eDataType m_indexType;
  cs::ePrimitiveType m_primitiveType;

  cs::BoundingBox m_boundingBox;
};

CS_CLASS()
class CSE_API SubMeshWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::SubMesh, SubMeshWrapper, cs::ResourceWrapper);
};


}

