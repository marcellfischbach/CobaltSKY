#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/imesh.hh>
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
class csSubMeshWrapper;



const csUInt32 csInvalidMaterialIndex = ~0x00;

CS_CLASS()
class CSE_API csMesh : public CS_SUPER(iMesh)
{
  CS_CLASS_GEN_OBJECT;
public:
  csMesh();
  virtual ~csMesh();

  void AddMesh(csSubMeshWrapper *instance, csSize materialIndex = 0, csUInt8 lod = 0, const std::string &name = "");
  csUInt8 GetNumberOfLODs() const;
  csSize GetNumberOfMeshes(csUInt8 lod = 0) const;
  csSubMeshWrapper *GetMesh(csUInt8 lod = 0, csSize idx = 0);
  const csSubMeshWrapper *GetMesh(csUInt8 lod = 0, csSize idx = 0) const;

  const csBoundingBox &GetBoundingBox() const;

  csSize GetMaterialIndex(csUInt8 lod = 0, csSize idx = 0) const;
  void OptimizeDataStruct();
  void UpdateBoundingBox();


  csSize GetNumberOfMaterials() const;
  void AddMaterialName(const std::string &materialName);
  const std::string &GetMaterialName(csSize idx) const;
  csUInt32 GetMaterialIndex(const std::string &materialName) const;

  virtual void Update(iGraphics *renderer, const csVector3f &cameraPos, csUInt64 frameNo);
  virtual void Render(iGraphics *renderer, csRenderPass pass, const std::vector<csMaterial *> &materials, csUInt8 lod = 0);

  csSize GetNumberOfRenderCalls(csUInt8 lod = 0) const;
  csSize GetNumberOfTotalTrigons(csUInt8 lod = 0) const;


protected:
  struct Data
  {
    csSubMeshWrapper *m_mesh;
    csSize m_materialIndex;
    std::string m_name;
  };

  struct LOD
  {
    std::vector<Data> m_meshes;
  };

  std::vector<LOD> m_lods;
  csSize m_numberOfMaterials;
  std::vector<std::string> m_materialNames;

  LOD &GetLOD(csUInt8 lod);

  csBoundingBox m_boundingBox;
};

CS_CLASS()
class CSE_API csMeshWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csMesh, csMeshWrapper, csResourceWrapper);
};



CS_FORCEINLINE csSize csMesh::GetNumberOfMaterials() const
{
  return m_numberOfMaterials;
}

CS_FORCEINLINE const csBoundingBox &csMesh::GetBoundingBox() const
{
  return m_boundingBox;
}




