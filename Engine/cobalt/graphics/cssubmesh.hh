#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/graphics/cssubmesh.refl.hh>



struct iGraphics;
struct iIndexBuffer;
struct iVertexBuffer;
struct iVertexDeclaration;



CS_CLASS()
class CSE_API csSubMesh : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  csSubMesh();
  virtual ~csSubMesh();

  virtual void Render(iGraphics *renderer);

  void SetPrimitiveType(csPrimitiveType type);
  void SetIndexType(csDataType indexType);
  void SetVertexDeclaration(iVertexDeclaration *vertexDeclaration);
  void AddVertexBuffer(iVertexBuffer *vertexBuffer);
  void SetIndexBuffer(iIndexBuffer *indexBuffer, csSize count, csSize offset = 0);

  void SetBoundingBox(const csBoundingBox &bbox);
  const csBoundingBox& GetBoundingBox() const;

  unsigned GetNumberOfTrigons() const;

private:

  iIndexBuffer *m_indexBuffer;
  csSize        m_offset;
  csSize        m_count;

  iVertexDeclaration *m_vertexDeclaration;
  std::vector<iVertexBuffer*> m_vertexBuffer;

  csDataType m_indexType;
  csPrimitiveType m_primitiveType;

  csBoundingBox m_boundingBox;
};

CS_CLASS()
class CSE_API csSubMeshWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csSubMesh, csSubMeshWrapper, csResourceWrapper);
};


