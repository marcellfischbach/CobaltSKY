#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/cstextureassetxmlloader.refl.hh>

class csImage;
struct iSampler;



/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API csTextureAssetXMLLoader : public CS_SUPER(csBaseXMLLoader)
{
  CS_CLASS_GEN;
public:
  csTextureAssetXMLLoader();
  virtual ~csTextureAssetXMLLoader();

  virtual bool CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  csTextureType GetTextureType(const csString &typeName) const;


  iObject *LoadTexture2D(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  iObject *LoadTexture2DArray(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  csImage *LoadImage(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
  iSampler *LoadSampler(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;
};


