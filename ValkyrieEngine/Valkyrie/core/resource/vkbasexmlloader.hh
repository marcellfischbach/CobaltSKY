#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/resource/ixmlloader.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Vector.hh>

#include <Valkyrie/core/resource/vkbasexmlloader.refl.hh>


VK_INTERFACE()
class VKE_API vkBaseXMLLoader : public VK_SUPER(IXMLLoader)
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
