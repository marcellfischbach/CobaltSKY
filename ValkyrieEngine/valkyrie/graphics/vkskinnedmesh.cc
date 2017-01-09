
#include <valkyrie/graphics/vkskinnedmesh.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/animation/vkskeleton.hh>



vkSkinnedMesh::vkSkinnedMesh()
  : vkMesh()
{

}


vkSkinnedMesh::~vkSkinnedMesh()
{

}



void vkSkinnedMesh::Render(iGraphics *renderer, vkRenderPass pass, vkSize numMaterials, vkMaterialInstance **material, vkUInt8 lod)
{
  if (m_boneIdxMapping && m_numberOfIndexMapping > 0)
  {
    renderer->SetSkeletonBoneMapping(m_boneIdxMapping, m_numberOfIndexMapping);

    vkMesh::Render(renderer, pass, numMaterials, material, lod);
  }
}




void vkSkinnedMesh::AddBoneName(const vkString &boneName, vkUInt8 boneIdx)
{
  m_boneNameMapping[boneName] = boneIdx;
}


void vkSkinnedMesh::GenerateMapping(const vkSkeleton *skeleton)
{
  if (m_boneIdxMapping)
  {
    delete[] m_boneIdxMapping;
  }
  m_numberOfIndexMapping = m_boneNameMapping.size();
  m_boneIdxMapping = new vkUInt32[m_numberOfIndexMapping];

  for (std::map<vkString, vkUInt8>::iterator it = m_boneNameMapping.begin(); it != m_boneNameMapping.end(); ++it)
  {
    vkSize boneIdx = skeleton->GetBoneIndex(it->first);
    if (boneIdx == vkInvalidBoneIdx)
    {
      continue;
    }

    m_boneIdxMapping[it->second] = (vkUInt32)boneIdx;

  }
}