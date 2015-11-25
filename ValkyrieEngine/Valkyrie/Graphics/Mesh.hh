#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <vector>
#include <Valkyrie/Graphics/Mesh.refl.hh>

struct IIndexBuffer;
struct IGraphics;
struct IVertexBuffer;
struct IVertexDeclaration;


VK_CLASS()
class VKE_API vkMesh : public vkObject
{
  VK_CLASS_GEN;

public:
  vkMesh();
  virtual ~vkMesh();

  void Render(IGraphics *renderer, vkUInt8 lod = 0);

  void SetPrimitiveType(vkPrimitiveType type);
  void SetIndexType(vkDataType indexType);
  void SetVertexDeclaration(IVertexDeclaration *vertexDeclaration);
  void AddVertexBuffer(IVertexBuffer *vertexBuffer);
  void AddIndexBuffer(IIndexBuffer *indexBuffer, vkSize count, vkSize offset = 0);

  void SetBoundingBox(const vkBoundingBox &bbox);
  const vkBoundingBox& GetBoundingBox() const;

private:

  struct Index
  {
    IIndexBuffer *m_indexBuffer;
    vkSize        m_offset;
    vkSize        m_count;
  };

  IVertexDeclaration *m_vertexDeclaration;
  std::vector<IVertexBuffer*> m_vertexBuffer;
  std::vector<Index> m_indices;

  vkDataType m_indexType;
  vkPrimitiveType m_primitiveType;
  
  vkBoundingBox m_boundingBox;
};

