#pragma once


#include <Valkyrie/Core/ResourceManager.hh>
#include <AssetManager/EditorIconLoader.refl.hh>


/*
VK_CLASS()
class EditorIconAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  EditorIconAssetLoader() : IAssetLoader() { }
  virtual ~EditorIconAssetLoader() { }

  virtual bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0);

  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);

};
*/

VK_CLASS()
class EditorIconAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  EditorIconAssetXMLLoader() : vkBaseXMLLoader() { }
  virtual ~EditorIconAssetXMLLoader() { }


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
};
