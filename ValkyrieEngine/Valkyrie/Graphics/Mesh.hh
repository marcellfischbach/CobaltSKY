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


class vkMaterialInstance;

VK_CLASS()
class VKE_API vkSubMesh : public vkObject
{
  VK_CLASS_GEN;

public:
  vkSubMesh();
  virtual ~vkSubMesh();

  void Render(IGraphics *renderer);

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




VK_CLASS()
class VKE_API vkMesh : public vkObject
{
  VK_CLASS_GEN;
public:
  vkMesh();
  virtual ~vkMesh();

  void AddMesh(vkSubMesh *instance, vkSize materialIndex = 0, vkUInt8 lod = 0, const vkString &name = "");
  vkUInt8 GetNumberOfLODs() const;
  vkSize GetNumberOfMeshes(vkUInt8 lod = 0) const;
  vkSubMesh *GetMesh(vkUInt8 lod = 0, vkSize idx = 0);
  const vkSubMesh *GetMesh(vkUInt8 lod = 0, vkSize idx = 0) const;

  const vkBoundingBox &GetBoundingBox() const;

  vkSize GetMaterialIndex(vkUInt8 lod = 0, vkSize idx = 0) const;
  void OptimizeDataStruct();
  void UpdateBoundingBox();


  vkUInt32 GetNumberOfMaterials() const;

  void Render(IGraphics *renderer, vkRenderPass pass, vkUInt32 numMaterials, vkMaterialInstance **material, vkUInt8 lod = 0);


private:
  struct Data
  {
    vkSubMesh *m_mesh;
    vkSize m_materialIndex;
    vkString m_name;
  };

  struct LOD
  {
    std::vector<Data> m_meshes;
  };

  std::vector<LOD> m_lods;
  vkUInt32 m_numberOfMaterials;

  LOD &GetLOD(vkUInt8 lod);

  vkBoundingBox m_boundingBox;
};


VK_FORCEINLINE vkUInt32 vkMesh::GetNumberOfMaterials() const
{
  return m_numberOfMaterials;
}

VK_FORCEINLINE const vkBoundingBox &vkMesh::GetBoundingBox() const
{
  return m_boundingBox;
}

