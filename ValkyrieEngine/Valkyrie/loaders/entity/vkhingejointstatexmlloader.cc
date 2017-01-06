
#include <Valkyrie/loaders/entity/vkhingejointstatexmlloader.hh>
#include <Valkyrie/entity/vkhingejointstate.hh>


vkHingeJointStateXMLLoader::vkHingeJointStateXMLLoader()
  : vkJointStateXMLLoader()
{

}

vkHingeJointStateXMLLoader::~vkHingeJointStateXMLLoader()
{

}

const vkClass *vkHingeJointStateXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkHingeJointState::GetStaticClass();
}


IObject *vkHingeJointStateXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkJointStateXMLLoader::Load(element, locator, userData);
}

const vkClass *vkHingeJointStateXMLLoader::GetLoadingClass() const
{
  return vkHingeJointState::GetStaticClass();
}

