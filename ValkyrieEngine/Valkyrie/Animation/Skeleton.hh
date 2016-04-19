#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Animation/Skeleton.refl.hh>

VK_CLASS()
class VKE_API vkSkeleton : public vkObject
{
  VK_CLASS_GEN;
public:
  vkSkeleton();
  virtual ~vkSkeleton();

  void PrepareBones(vkSize numberOfBones);

  vkSize GetNumberOfBones() const;

  vkMatrix4f *GetMatrices();
  const vkMatrix4f *GetMatrices() const;

private:
  vkSize m_numberOfBones;
  vkMatrix4f *m_matrices;

};

VK_FORCEINLINE vkSize vkSkeleton::GetNumberOfBones() const
{
  return m_numberOfBones;
}

VK_FORCEINLINE vkMatrix4f *vkSkeleton::GetMatrices()
{
  return m_matrices;
}


VK_FORCEINLINE const vkMatrix4f *vkSkeleton::GetMatrices() const
{
  return m_matrices;
}

