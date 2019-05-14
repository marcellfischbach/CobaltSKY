#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/resource/icsfloader.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <cobalt/math/csvector4f.hh>

#include <cobalt/core/resource/csbasecsfloader.refl.hh>


CS_CLASS()
class CSE_API csBaseCSFLoader : public CS_SUPER(iCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
protected:
  csBaseCSFLoader();
  virtual ~csBaseCSFLoader();

  const csfEntry *FindEntry(const csfEntry *root, const std::string &entryName, const std::string &name = "") const;
  const csfEntry *FindEntryByTagName(const csfEntry *root, const std::string &entryName) const;

  bool HasLocator (const csfEntry *entry) const;
  csResourceLocator GetLocator(const csfEntry *entry) const;
  csResourceLoadingMode GetResourceLoadingMode(const csfEntry *entry, csResourceLoadingMode defaultMode = eRLM_Shared, csResourceLoadingMode alterInline = eRLM_Inline) const;

  bool LoadBool(const csfEntry *entry, size_t idx = 0) const;
  bool LoadBool(const csfEntry *entry, const std::string &attributeName) const;
  float LoadFloat(const csfEntry *entry, size_t offset = 0) const;
  float LoadFloat(const csfEntry *entry, const std::string &firstAttributeName) const;
  csVector2f LoadVector2f(const csfEntry *entry, size_t offset = 0) const;
  csVector2f LoadVector2f(const csfEntry *entry, const std::string &firstAttributeName) const;
  csVector3f LoadVector3f(const csfEntry *entry, size_t offset = 0) const;
  csVector3f LoadVector3f(const csfEntry *entry, const std::string &firstAttributeName) const;
  csVector4f LoadVector4f(const csfEntry *entry, size_t offset = 0) const;
  csVector4f LoadVector4f(const csfEntry *entry, const std::string &firstAttributeName) const;
  csColor4f LoadColor4f(const csfEntry *entry, size_t offset = 0) const;
  csColor4f LoadColor4f(const csfEntry *entry, const std::string &firstAttributeName5) const;
};
