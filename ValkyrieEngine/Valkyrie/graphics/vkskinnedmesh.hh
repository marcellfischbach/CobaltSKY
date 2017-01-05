#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/vkmesh.hh>
#include <Valkyrie/graphics/vkskinnedmesh.refl.hh>

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

