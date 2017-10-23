
#include <cobalt/loaders/entity/cshingejointstatecsfloader.hh>
#include <cobalt/entity/cshingejointstate.hh>


csHingeJointStateCSFLoader::csHingeJointStateCSFLoader()
  : csJointStateCSFLoader()
{

}

csHingeJointStateCSFLoader::~csHingeJointStateCSFLoader()
{

}

const csClass *csHingeJointStateCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csHingeJointState::GetStaticClass();
}


iObject *csHingeJointStateCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csJointStateCSFLoader::Load(entry, locator, userData);
}

const csClass *csHingeJointStateCSFLoader::GetLoadingClass() const
{
  return csHingeJointState::GetStaticClass();
}

