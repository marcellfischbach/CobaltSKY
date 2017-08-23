#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/resource/ixmlloader.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/math/csvector4f.hh>

#include <cobalt/core/resource/csbasexmlloader.refl.hh>


CS_INTERFACE()
class CSE_API csBaseXMLLoader : public CS_SUPER(iXMLLoader)
{
  CS_CLASS_GEN_OBJECT;
protected:
  csBaseXMLLoader();
  virtual ~csBaseXMLLoader();

  TiXmlElement *FindElement(TiXmlElement *root, const std::string &elementName, const std::string &name = "") const;
  TiXmlElement *FindElementByTagName(TiXmlElement *root, const std::string &elementName) const;

  csResourceLoadingMode GetResourceLoadingMode(TiXmlElement *element, csResourceLoadingMode defaultMode = eRLM_Shared, csResourceLoadingMode alterInline = eRLM_Inline) const;

  bool LoadBool(const char *str) const;
  float LoadFloat(const char *str) const;
  csVector2f LoadVector2f(const char *str) const;
  csVector3f LoadVector3f(const char *str) const;
  csVector4f LoadVector4f(const char *str) const;
  csColor4f LoadColor4f(const char *str) const;
};
