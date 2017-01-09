
#include <valkyrie/core/resource/vkassetxmlloader.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <tixml/tinyxml.h>

vkAssetXMLLoader::vkAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

vkAssetXMLLoader::~vkAssetXMLLoader()
{

}

bool vkAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("asset");
}


const vkClass *vkAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
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

iObject *vkAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
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