#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/TextureLoader.refl.hh>

class vkImage;
struct ISampler;

/**
* \ingroup loading
*/

VK_CLASS()
class VKE_API vkSamplerAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkSamplerAssetXMLLoader();
  virtual ~vkSamplerAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};


/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkTextureAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkTextureAssetXMLLoader();
  virtual ~vkTextureAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  vkTextureType GetTextureType(const vkString &typeName) const;


  IObject *LoadTexture2D(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *LoadTexture2DArray(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  vkImage *LoadImage(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  ISampler *LoadSampler(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
};



VK_INTERFACE();
struct VKE_API ITextureAssetImageLoader : public IObject
{
  VK_CLASS_GEN;
  
  virtual bool CanLoad(const vkString &imageTypeID) = 0;
  virtual vkImage *Load(vkAssetInputStream &inputStream) = 0;
};

VK_CLASS();
class VKE_API vkTextureAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkTextureAssetLoader();
  virtual ~vkTextureAssetLoader();

  virtual bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0);
  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);


private:
  std::vector<ITextureAssetImageLoader*> m_imageLoaders;
  ITextureAssetImageLoader *FindImageLoader(const vkString &imageTypeID);

};



VK_CLASS();
class VKE_API vkTextureAssetImagePNGLoader : public ITextureAssetImageLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkTextureAssetImagePNGLoader();
  virtual ~vkTextureAssetImagePNGLoader();

  virtual bool CanLoad(const vkString &imageTypeID);
  virtual vkImage *Load(vkAssetInputStream &inputStream);
};