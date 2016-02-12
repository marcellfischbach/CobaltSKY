

#include <Valkyrie/Animation/Skeleton.hh>


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
  }
  m_matrices = new vkMatrix4f[numberOfBones];
  m_numberOfBones = numberOfBones;
}

