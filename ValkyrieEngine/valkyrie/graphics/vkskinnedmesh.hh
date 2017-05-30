#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/vkmesh.hh>
#include <valkyrie/graphics/vkskinnedmesh.refl.hh>

VK_CLASS()
class VKE_API vkSkinnedMesh : public VK_SUPER(vkMesh)
{
  VK_CLASS_GEN;

public:
  vkSkinnedMesh();
  virtual ~vkSkinnedMesh();

  void AddBoneName(const vkString &boneName, vkUInt8 boneIndex);
  void GenerateMapping(const vkSkeleton *skeleton);

  virtual void Render(iGraphics *renderer, vkRenderPass pass, vkSize numMaterials, vkMaterial **material, vkUInt8 lod = 0) override;

private:
  std::map<vkString, vkUInt8> m_boneNameMapping;

  vkUInt32 *m_boneIdxMapping;
  vkSize m_numberOfIndexMapping;

};

