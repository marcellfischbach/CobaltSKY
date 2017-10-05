#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/resource/icsfloader.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/math/csvector4f.hh>

#include <cobalt/core/resource/csbasecsfloader.refl.hh>


CS_INTERFACE()
class CSE_API csBaseCSFLoader : public CS_SUPER(iCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
protected:
  csBaseCSFLoader();
  virtual ~csBaseCSFLoader();

  const csfEntry *FindEntry(const csfEntry *root, const std::string &entryName, const std::string &name = "") const;
  const csfEntry *FindEntryByTagName(const csfEntry *root, const std::string &entryName) const;

  csResourceLoadingMode GetResourceLoadingMode(const csfEntry *entry, csResourceLoadingMode defaultMode = eRLM_Shared, csResourceLoadingMode alterInline = eRLM_Inline) const;

  bool LoadBool(const csfEntry *entry) const;
  float LoadFloat(const csfEntry *entry) const;
  csVector2f LoadVector2f(const csfEntry *entry) const;
  csVector3f LoadVector3f(const csfEntry *entry) const;
  csVector4f LoadVector4f(const csfEntry *entry) const;
  csColor4f LoadColor4f(const csfEntry *entry) const;
};
