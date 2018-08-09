#pragma once


#include <Valkyrie/Core/ResourceManager.hh>
#include <ShaderGraph/MetaDataLoader.refl.hh>


VK_CLASS()
class MetaDataLoader : public VK_SUPER(IAssetLoader)
{
  VK_CLASS_GEN_OBJECT;
public:
  MetaDataLoader();
  virtual ~MetaDataLoader();

  virtual bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;

};


VK_CLASS()
class ShaderGraphMetaDataAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  ShaderGraphMetaDataAssetXMLLoader();
  virtual ~ShaderGraphMetaDataAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
};