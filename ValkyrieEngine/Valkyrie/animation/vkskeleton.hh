#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/animation/vkskeleton.refl.hh>

const vkSize vkInvalidBoneIdx = ~0x00;

VK_CLASS()
class VKE_API vkSkeleton : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkSkeleton();
  virtual ~vkSkeleton();

  void PrepareBones(vkSize numberOfBones);

  void SetBoneName(vkSize boneIdx, const vkString &boneName);

  vkSize GetNumberOfBones() const;

  vkMatrix4f *GetMatrices();
  const vkMatrix4f *GetMatrices() const;

  vkString GetBoneName(vkSize boneIdx) const;
  vkSize GetBoneIndex(const vkString &boneIndex) const;

private:
  vkSize m_numberOfBones;
  vkMatrix4f *m_matrices;
  vkString *m_boneNames;

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

