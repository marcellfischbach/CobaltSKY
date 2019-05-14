

#include <cobalt/animation/csskeleton.hh>


csSkeleton::csSkeleton()
  : cs::Object()
  , m_numberOfBones(0)
  , m_matrices(0)
{

}

csSkeleton::~csSkeleton()
{
  if (m_matrices)
  {
    delete[] m_matrices;
  }
}


void csSkeleton::PrepareBones(csSize numberOfBones)
{
  if (m_matrices)
  {
    delete[] m_matrices;
    delete[] m_boneNames;
  }
  m_matrices = new csMatrix4f[numberOfBones];
  m_boneNames = new std::string[numberOfBones];
  m_numberOfBones = numberOfBones;
}



void csSkeleton::SetBoneName(csSize boneIdx, const std::string &boneName)
{
  if (boneIdx >= m_numberOfBones)
  {
    return;
  }

  m_boneNames[boneIdx] = boneName;
}


std::string csSkeleton::GetBoneName(csSize idx) const
{
  if (idx >= m_numberOfBones)
  {
    return "";
  }

  return m_boneNames[idx];
}

csSize csSkeleton::GetBoneIndex(const std::string &boneName) const
{
  for (csSize i = 0; i < m_numberOfBones; ++i)
  {
    if (m_boneNames[i] == boneName)
    {
      return i;
    }
  }
  return csInvalidBoneIdx;
}


