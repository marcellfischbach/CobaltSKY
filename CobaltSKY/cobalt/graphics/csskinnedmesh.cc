
#include <cobalt/graphics/csskinnedmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/animation/csskeleton.hh>



csSkinnedMesh::csSkinnedMesh()
  : csMesh()
{

}


csSkinnedMesh::~csSkinnedMesh()
{

}



void csSkinnedMesh::Render(iGraphics *renderer, csRenderPass pass, csSize numMaterials, csMaterial **material, csUInt8 lod)
{
  if (m_boneIdxMapping && m_numberOfIndexMapping > 0)
  {
    renderer->SetSkeletonBoneMapping(m_boneIdxMapping, m_numberOfIndexMapping);

    csMesh::Render(renderer, pass, numMaterials, material, lod);
  }
}




void csSkinnedMesh::AddBoneName(const std::string &boneName, csUInt8 boneIdx)
{
  m_boneNameMapping[boneName] = boneIdx;
}


void csSkinnedMesh::GenerateMapping(const csSkeleton *skeleton)
{
  if (m_boneIdxMapping)
  {
    delete[] m_boneIdxMapping;
  }
  m_numberOfIndexMapping = m_boneNameMapping.size();
  m_boneIdxMapping = new csUInt32[m_numberOfIndexMapping];

  for (std::map<std::string, csUInt8>::iterator it = m_boneNameMapping.begin(); it != m_boneNameMapping.end(); ++it)
  {
    csSize boneIdx = skeleton->GetBoneIndex(it->first);
    if (boneIdx == csInvalidBoneIdx)
    {
      continue;
    }

    m_boneIdxMapping[it->second] = (csUInt32)boneIdx;

  }
}