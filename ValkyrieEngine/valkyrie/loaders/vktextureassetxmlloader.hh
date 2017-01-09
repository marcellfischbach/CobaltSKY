#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vktextureassetxmlloader.refl.hh>

class vkImage;
struct iSampler;



/**
* \ingroup loading
*/
VK_CLASS()
class VKE_API vkTextureAssetXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkTextureAssetXMLLoader();
  virtual ~vkTextureAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

private:
  vkTextureType GetTextureType(const vkString &typeName) const;


  iObject *LoadTexture2D(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  iObject *LoadTexture2DArray(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  vkImage *LoadImage(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
  iSampler *LoadSampler(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;
};


