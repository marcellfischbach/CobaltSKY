#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <map>
#include <vector>
#include <Valkyrie/Graphics/Mesh.refl.hh>

struct IIndexBuffer;
struct IGraphics;
struct IVertexBuffer;
struct IVertexDeclaration;

class vkMaterialInstance;
class vkSkeleton;

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



const vkUInt32 vkInvalidMaterialIndex = ~0x00;

VK_CLASS()
class VKE_API vkMesh : public VK_SUPER(vkObject)
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


  vkSize GetNumberOfMaterials() const;
  void AddMaterialName(const vkString &materialName);
  const vkString &GetMaterialName(vkSize idx) const;
  vkUInt32 GetMaterialIndex(const vkString &materialName) const;

  virtual void Render(IGraphics *renderer, vkRenderPass pass, vkSize numMaterials, vkMaterialInstance **material, vkUInt8 lod = 0);


protected:
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
  vkSize m_numberOfMaterials;
  std::vector<vkString> m_materialNames;

  LOD &GetLOD(vkUInt8 lod);

  vkBoundingBox m_boundingBox;
};




VK_CLASS()
class VKE_API vkSkinnedMesh : public VK_SUPER(vkMesh)
{
  VK_CLASS_GEN;

public:
  vkSkinnedMesh();
  virtual ~vkSkinnedMesh();

  void AddBoneName(const vkString &boneName, vkUInt8 boneIndex);
  void GenerateMapping(const vkSkeleton *skeleton);

  virtual void Render(IGraphics *renderer, vkRenderPass pass, vkSize numMaterials, vkMaterialInstance **material, vkUInt8 lod = 0) override;

private:
  std::map<vkString, vkUInt8> m_boneNameMapping;

  vkUInt32 *m_boneIdxMapping;
  vkSize m_numberOfIndexMapping;

};


VK_FORCEINLINE vkSize vkMesh::GetNumberOfMaterials() const
{
  return m_numberOfMaterials;
}

VK_FORCEINLINE const vkBoundingBox &vkMesh::GetBoundingBox() const
{
  return m_boundingBox;
}




