#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <map>
#include <vector>
#include <Valkyrie/graphics/vkmesh.refl.hh>

struct IIndexBuffer;
struct IGraphics;
struct IVertexBuffer;
struct IVertexDeclaration;

class vkMaterialInstance;
class vkSkeleton;
class vkSubMesh;



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





VK_FORCEINLINE vkSize vkMesh::GetNumberOfMaterials() const
{
  return m_numberOfMaterials;
}

VK_FORCEINLINE const vkBoundingBox &vkMesh::GetBoundingBox() const
{
  return m_boundingBox;
}




