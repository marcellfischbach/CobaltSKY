

#include <cobalt/animation/csskeleton.hh>


cs::Skeleton::Skeleton()
  : cs::Object()
  , m_numberOfBones(0)
  , m_matrices(0)
  , m_boneNames(nullptr)
{

}

cs::Skeleton::~Skeleton()
{
  if (m_matrices)
  {
    delete[] m_matrices;
  }
}


void cs::Skeleton::PrepareBones(csSize numberOfBones)
{
  if (m_matrices)
  {
    delete[] m_matrices;
    delete[] m_boneNames;
  }
  m_matrices = new cs::Matrix4f[numberOfBones];
  m_boneNames = new std::string[numberOfBones];
  m_numberOfBones = numberOfBones;
}



void cs::Skeleton::SetBoneName(csSize boneIdx, const std::string &boneName)
{
  if (boneIdx >= m_numberOfBones)
  {
    return;
  }

  m_boneNames[boneIdx] = boneName;
}


std::string cs::Skeleton::GetBoneName(csSize idx) const
{
  if (idx >= m_numberOfBones)
  {
    return "";
  }

  return m_boneNames[idx];
}

csSize cs::Skeleton::GetBoneIndex(const std::string &boneName) const
{
  for (csSize i = 0; i < m_numberOfBones; ++i)
  {
    if (m_boneNames[i] == boneName)
    {
      return i;
    }
  }
  return cs::InvalidBoneIdx;
}


