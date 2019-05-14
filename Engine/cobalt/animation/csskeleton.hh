#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/animation/csskeleton.refl.hh>

const csSize csInvalidBoneIdx = ~0x00;

CS_CLASS()
class CSE_API csSkeleton : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  csSkeleton();
  virtual ~csSkeleton();

  void PrepareBones(csSize numberOfBones);

  void SetBoneName(csSize boneIdx, const std::string &boneName);

  csSize GetNumberOfBones() const;

  csMatrix4f *GetMatrices();
  const csMatrix4f *GetMatrices() const;

  std::string GetBoneName(csSize boneIdx) const;
  csSize GetBoneIndex(const std::string &boneIndex) const;

private:
  csSize m_numberOfBones;
  csMatrix4f *m_matrices;
  std::string *m_boneNames;

};

CS_FORCEINLINE csSize csSkeleton::GetNumberOfBones() const
{
  return m_numberOfBones;
}

CS_FORCEINLINE csMatrix4f *csSkeleton::GetMatrices()
{
  return m_matrices;
}


CS_FORCEINLINE const csMatrix4f *csSkeleton::GetMatrices() const
{
  return m_matrices;
}

