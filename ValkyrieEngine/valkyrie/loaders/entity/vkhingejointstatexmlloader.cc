
#include <valkyrie/loaders/entity/vkhingejointstatexmlloader.hh>
#include <valkyrie/entity/vkhingejointstate.hh>


vkHingeJointStateXMLLoader::vkHingeJointStateXMLLoader()
  : vkJointStateXMLLoader()
{

}

vkHingeJointStateXMLLoader::~vkHingeJointStateXMLLoader()
{

}

const vkClass *vkHingeJointStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkHingeJointState::GetStaticClass();
}


iObject *vkHingeJointStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkJointStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkHingeJointStateXMLLoader::GetLoadingClass() const
{
  return vkHingeJointState::GetStaticClass();
}

