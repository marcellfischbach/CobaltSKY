
#include <cobalt/graphics/csskinnedmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/animation/csskeleton.hh>



cs::SkinnedMesh::SkinnedMesh()
  : cs::Mesh()
{

}


cs::SkinnedMesh::~SkinnedMesh()
{

}



void cs::SkinnedMesh::Render(cs::iGraphics *renderer, cs::eRenderPass pass, const std::vector<cs::Material *> &materials, csUInt8 lod)
{
  if (m_boneIdxMapping && m_numberOfIndexMapping > 0)
  {
    renderer->SetSkeletonBoneMapping(m_boneIdxMapping, m_numberOfIndexMapping);

    cs::Mesh::Render(renderer, pass, materials, lod);
  }
}




void cs::SkinnedMesh::AddBoneName(const std::string &boneName, csUInt8 boneIdx)
{
  m_boneNameMapping[boneName] = boneIdx;
}


void cs::SkinnedMesh::GenerateMapping(const cs::Skeleton *skeleton)
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
    if (boneIdx == cs::InvalidBoneIdx)
    {
      continue;
    }

    m_boneIdxMapping[it->second] = (csUInt32)boneIdx;

  }
}
