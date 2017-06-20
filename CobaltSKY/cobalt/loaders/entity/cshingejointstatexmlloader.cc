
#include <cobalt/loaders/entity/cshingejointstatexmlloader.hh>
#include <cobalt/entity/cshingejointstate.hh>


csHingeJointStateXMLLoader::csHingeJointStateXMLLoader()
  : csJointStateXMLLoader()
{

}

csHingeJointStateXMLLoader::~csHingeJointStateXMLLoader()
{

}

const csClass *csHingeJointStateXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csHingeJointState::GetStaticClass();
}


iObject *csHingeJointStateXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csJointStateXMLLoader::Load(element, locator, userData);
}

const csClass *csHingeJointStateXMLLoader::GetLoadingClass() const
{
  return csHingeJointState::GetStaticClass();
}

