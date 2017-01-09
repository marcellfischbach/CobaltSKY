#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkboundingbox.hh>
#include <valkyrie/graphics/vksubmesh.refl.hh>



struct iGraphics;
struct iIndexBuffer;
struct iVertexBuffer;
struct iVertexDeclaration;



VK_CLASS()
class VKE_API vkSubMesh : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  vkSubMesh();
  virtual ~vkSubMesh();

  virtual void Render(iGraphics *renderer);

  void SetPrimitiveType(vkPrimitiveType type);
  void SetIndexType(vkDataType indexType);
  void SetVertexDeclaration(iVertexDeclaration *vertexDeclaration);
  void AddVertexBuffer(iVertexBuffer *vertexBuffer);
  void SetIndexBuffer(iIndexBuffer *indexBuffer, vkSize count, vkSize offset = 0);

  void SetBoundingBox(const vkBoundingBox &bbox);
  const vkBoundingBox& GetBoundingBox() const;

private:

  iIndexBuffer *m_indexBuffer;
  vkSize        m_offset;
  vkSize        m_count;

  iVertexDeclaration *m_vertexDeclaration;
  std::vector<iVertexBuffer*> m_vertexBuffer;

  vkDataType m_indexType;
  vkPrimitiveType m_primitiveType;

  vkBoundingBox m_boundingBox;
};
