
#include <Valkyrie/core/resource/vkassetxmlloader.hh>
#include <Valkyrie/core/vkresourcelocator.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <tixml/tinyxml.h>

vkAssetXMLLoader::vkAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

vkAssetXMLLoader::~vkAssetXMLLoader()
{

}

bool vkAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("asset");
}


const vkClass *vkAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *dataElement = FindElementByTagName(element, locator.GetResourceName());
  if (!dataElement)
  {
    return 0;
  }

  TiXmlElement *firstChild = dataElement->FirstChildElement();
  if (!firstChild)
  {
    return 0;
  }

  return vkResourceManager::Get()->EvalClass(firstChild, locator);
}

IObject *vkAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *dataElement = FindElementByTagName(element, locator.GetResourceName());
  if (!dataElement)
  {
    return 0;
  }

  TiXmlElement *firstChild = dataElement->FirstChildElement();
  if (!firstChild)
  {
    return 0;
  }

  return vkResourceManager::Get()->Load(firstChild, locator);
}