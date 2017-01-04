

#include <Valkyrie/animation/vkskeleton.hh>


vkSkeleton::vkSkeleton()
  : vkObject()
  , m_numberOfBones(0)
  , m_matrices(0)
{

}

vkSkeleton::~vkSkeleton()
{
  if (m_matrices)
  {
    delete[] m_matrices;
  }
}


void vkSkeleton::PrepareBones(vkSize numberOfBones)
{
  if (m_matrices)
  {
    delete[] m_matrices;
    delete[] m_boneNames;
  }
  m_matrices = new vkMatrix4f[numberOfBones];
  m_boneNames = new vkString[numberOfBones];
  m_numberOfBones = numberOfBones;
}



void vkSkeleton::SetBoneName(vkSize boneIdx, const vkString &boneName)
{
  if (boneIdx >= m_numberOfBones)
  {
    return;
  }

  m_boneNames[boneIdx] = boneName;
}


vkString vkSkeleton::GetBoneName(vkSize idx) const
{
  if (idx >= m_numberOfBones)
  {
    return "";
  }

  return m_boneNames[idx];
}

vkSize vkSkeleton::GetBoneIndex(const vkString &boneName) const
{
  for (vkSize i = 0; i < m_numberOfBones; ++i)
  {
    if (m_boneNames[i] == boneName)
    {
      return i;
    }
  }
  return vkInvalidBoneIdx;
}


