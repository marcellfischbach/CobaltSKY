#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/csskinnedmesh.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API SkinnedMesh : public CS_SUPER(cs::Mesh)
{
  CS_CLASS_GEN;

public:
  SkinnedMesh();
  virtual ~SkinnedMesh();

  void AddBoneName(const std::string & boneName, csUInt8 boneIndex);
  void GenerateMapping(const cs::Skeleton * skeleton);

  virtual void Render(cs::iGraphics * renderer, cs::eRenderPass pass, const std::vector<cs::Material*> & materials, csUInt8 lod = 0) override;

private:
  std::map<std::string, csUInt8> m_boneNameMapping;

  csUInt32 * m_boneIdxMapping;
  csSize m_numberOfIndexMapping;

};

CS_CLASS()
class CSE_API SkinnedMeshWrapper : public CS_SUPER(cs::MeshWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::SkinnedMesh, SkinnedMeshWrapper, cs::MeshWrapper);
};

}
