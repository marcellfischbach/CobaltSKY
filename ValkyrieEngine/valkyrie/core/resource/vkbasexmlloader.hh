#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/resource/ixmlloader.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkvector2f.hh>
#include <valkyrie/math/vkvector3f.hh>
#include <valkyrie/math/vkvector4f.hh>

#include <valkyrie/core/resource/vkbasexmlloader.refl.hh>


VK_INTERFACE()
class VKE_API vkBaseXMLLoader : public VK_SUPER(iXMLLoader)
{
  VK_CLASS_GEN_OBJECT;
protected:
  vkBaseXMLLoader();
  virtual ~vkBaseXMLLoader();

  TiXmlElement *FindElement(TiXmlElement *root, const vkString &elementName, const vkString &name = "") const;
  TiXmlElement *FindElementByTagName(TiXmlElement *root, const vkString &elementName) const;

  vkResourceLoadingMode GetResourceLoadingMode(TiXmlElement *element, vkResourceLoadingMode defaultMode = eRLM_Shared, vkResourceLoadingMode alterInline = eRLM_Inline) const;

  bool LoadBool(const char *str) const;
  float LoadFloat(const char *str) const;
  vkVector2f LoadVector2f(const char *str) const;
  vkVector3f LoadVector3f(const char *str) const;
  vkVector4f LoadVector4f(const char *str) const;
  vkColor4f LoadColor4f(const char *str) const;
};
