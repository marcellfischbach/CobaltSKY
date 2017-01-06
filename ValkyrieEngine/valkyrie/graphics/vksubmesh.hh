#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkboundingbox.hh>
#include <valkyrie/graphics/vksubmesh.refl.hh>



struct IGraphics;
struct IIndexBuffer;
struct IVertexBuffer;
struct IVertexDeclaration;



VK_CLASS()
class VKE_API vkSubMesh : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  vkSubMesh();
  virtual ~vkSubMesh();

  virtual void Render(IGraphics *renderer);

  void SetPrimitiveType(vkPrimitiveType type);
  void SetIndexType(vkDataType indexType);
  void SetVertexDeclaration(IVertexDeclaration *vertexDeclaration);
  void AddVertexBuffer(IVertexBuffer *vertexBuffer);
  void SetIndexBuffer(IIndexBuffer *indexBuffer, vkSize count, vkSize offset = 0);

  void SetBoundingBox(const vkBoundingBox &bbox);
  const vkBoundingBox& GetBoundingBox() const;

private:

  IIndexBuffer *m_indexBuffer;
  vkSize        m_offset;
  vkSize        m_count;

  IVertexDeclaration *m_vertexDeclaration;
  std::vector<IVertexBuffer*> m_vertexBuffer;

  vkDataType m_indexType;
  vkPrimitiveType m_primitiveType;

  vkBoundingBox m_boundingBox;
};
