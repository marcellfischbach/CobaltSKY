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


class vkMultiMaterial;

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

  vkSize GetMaterialIndex(vkUInt8 lod = 0, vkSize idx = 0) const;
  void OptimizeDataStruct();

  void Render(IGraphics *renderer, vkRenderPass pass, vkMultiMaterial *material, vkUInt8 lod = 0);


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

  LOD &GetLOD(vkUInt8 lod);
};
