#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/csskinnedmesh.refl.hh>

CS_CLASS()
class CSE_API csSkinnedMesh : public CS_SUPER(csMesh)
{
  CS_CLASS_GEN;

public:
  csSkinnedMesh();
  virtual ~csSkinnedMesh();

  void AddBoneName(const std::string &boneName, csUInt8 boneIndex);
  void GenerateMapping(const csSkeleton *skeleton);

  virtual void Render(iGraphics *renderer, csRenderPass pass, const std::vector<csMaterial *> &materials, csUInt8 lod = 0) override;

private:
  std::map<std::string, csUInt8> m_boneNameMapping;

  csUInt32 *m_boneIdxMapping;
  csSize m_numberOfIndexMapping;

};

CS_CLASS()
class CSE_API csSkinnedMeshWrapper : public CS_SUPER(csMeshWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csSkinnedMesh, csSkinnedMeshWrapper, csMeshWrapper);
};
