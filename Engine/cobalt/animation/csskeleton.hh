#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix4f.hh>
#include <cobalt/animation/csskeleton.refl.hh>

namespace cs
{

const csSize InvalidBoneIdx = ~0x00;

CS_CLASS()
class CSE_API Skeleton : public CS_SUPER(cs::Object)
{
	CS_CLASS_GEN;
public:
	Skeleton();
	virtual ~Skeleton();

	void PrepareBones(csSize numberOfBones);

	void SetBoneName(csSize boneIdx, const std::string & boneName);

	csSize GetNumberOfBones() const;

	cs::Matrix4f* GetMatrices();
	const cs::Matrix4f* GetMatrices() const;

	std::string GetBoneName(csSize boneIdx) const;
	csSize GetBoneIndex(const std::string & boneIndex) const;

private:
	csSize m_numberOfBones;
	cs::Matrix4f* m_matrices;
	std::string* m_boneNames;

};

CS_FORCEINLINE csSize Skeleton::GetNumberOfBones() const
{
	return m_numberOfBones;
}

CS_FORCEINLINE cs::Matrix4f* Skeleton::GetMatrices()
{
	return m_matrices;
}


CS_FORCEINLINE const cs::Matrix4f* Skeleton::GetMatrices() const
{
	return m_matrices;
}

}
