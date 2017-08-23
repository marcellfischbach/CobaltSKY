
#include <cobalt/core/resource/csassetxmlloader.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <tixml/tinyxml.h>

csAssetXMLLoader::csAssetXMLLoader()
  : csBaseXMLLoader()
{

}

csAssetXMLLoader::~csAssetXMLLoader()
{

}

bool csAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName(element->Value());
  return tagName == std::string("asset");
}


const csClass *csAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
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

  return csResourceManager::Get()->EvalClass(firstChild, locator);
}

iObject *csAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
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

  return csResourceManager::Get()->Load(firstChild, locator);
}