#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>

class vkImage;
struct ISampler;

/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkSamplerLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkSamplerLoader();
  virtual ~vkSamplerLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;


};



/**
* \ingroup loading
*/
VK_CLASS();
class VKE_API vkTextureLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkTextureLoader();
  virtual ~vkTextureLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  vkTextureType GetTextureType(TiXmlElement *element) const;


  IObject *LoadTexture2D(TiXmlElement *element) const;
  vkImage *LoadImage(TiXmlElement *element) const;
  ISampler *LoadSampler(TiXmlElement *element, bool &release) const;
};

