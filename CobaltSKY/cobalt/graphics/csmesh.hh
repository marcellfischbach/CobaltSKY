#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csboundingbox.hh>
#include <map>
#include <vector>
#include <cobalt/graphics/csmesh.refl.hh>

struct iIndexBuffer;
struct iGraphics;
struct iVertexBuffer;
struct iVertexDeclaration;

class csMaterial;
class csSkeleton;
class csSubMesh;



const csUInt32 csInvalidMaterialIndex = ~0x00;

CS_CLASS()
class CSE_API csMesh : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csMesh();
  virtual ~csMesh();

  void AddMesh(csSubMesh *instance, csSize materialIndex = 0, csUInt8 lod = 0, const csString &name = "");
  csUInt8 GetNumberOfLODs() const;
  csSize GetNumberOfMeshes(csUInt8 lod = 0) const;
  csSubMesh *GetMesh(csUInt8 lod = 0, csSize idx = 0);
  const csSubMesh *GetMesh(csUInt8 lod = 0, csSize idx = 0) const;

  const csBoundingBox &GetBoundingBox() const;

  csSize GetMaterialIndex(csUInt8 lod = 0, csSize idx = 0) const;
  void OptimizeDataStruct();
  void UpdateBoundingBox();


  csSize GetNumberOfMaterials() const;
  void AddMaterialName(const csString &materialName);
  const csString &GetMaterialName(csSize idx) const;
  csUInt32 GetMaterialIndex(const csString &materialName) const;

  virtual void Render(iGraphics *renderer, csRenderPass pass, csSize numMaterials, csMaterial **material, csUInt8 lod = 0);


protected:
  struct Data
  {
    csSubMesh *m_mesh;
    csSize m_materialIndex;
    csString m_name;
  };

  struct LOD
  {
    std::vector<Data> m_meshes;
  };

  std::vector<LOD> m_lods;
  csSize m_numberOfMaterials;
  std::vector<csString> m_materialNames;

  LOD &GetLOD(csUInt8 lod);

  csBoundingBox m_boundingBox;
};





CS_FORCEINLINE csSize csMesh::GetNumberOfMaterials() const
{
  return m_numberOfMaterials;
}

CS_FORCEINLINE const csBoundingBox &csMesh::GetBoundingBox() const
{
  return m_boundingBox;
}



