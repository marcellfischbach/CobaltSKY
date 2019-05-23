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

namespace cs
{

CS_CLASS()
class CSE_API BaseCSFLoader : public CS_SUPER(cs::iCSFLoader)
{
  CS_CLASS_GEN_OBJECT;
protected:
  BaseCSFLoader();
  virtual ~BaseCSFLoader();

  const cs::file::Entry* FindEntry(const cs::file::Entry * root, const std::string & entryName, const std::string & name = "") const;
  const cs::file::Entry * FindEntryByTagName(const cs::file::Entry * root, const std::string & entryName) const;

  bool HasLocator(const cs::file::Entry * entry) const;
  cs::ResourceLocator GetLocator(const cs::file::Entry * entry) const;
  cs::eResourceLoadingMode GetResourceLoadingMode(const cs::file::Entry * entry, cs::eResourceLoadingMode defaultMode = cs::eRLM_Shared, cs::eResourceLoadingMode alterInline = cs::eRLM_Inline) const;

  bool LoadBool(const cs::file::Entry * entry, size_t idx = 0) const;
  bool LoadBool(const cs::file::Entry * entry, const std::string & attributeName) const;
  float LoadFloat(const cs::file::Entry * entry, size_t offset = 0) const;
  float LoadFloat(const cs::file::Entry * entry, const std::string & firstAttributeName) const;
  cs::Vector2f LoadVector2f(const cs::file::Entry * entry, size_t offset = 0) const;
  cs::Vector2f LoadVector2f(const cs::file::Entry * entry, const std::string & firstAttributeName) const;
  cs::Vector3f LoadVector3f(const cs::file::Entry * entry, size_t offset = 0) const;
  cs::Vector3f LoadVector3f(const cs::file::Entry * entry, const std::string & firstAttributeName) const;
  cs::Vector4f LoadVector4f(const cs::file::Entry * entry, size_t offset = 0) const;
  cs::Vector4f LoadVector4f(const cs::file::Entry * entry, const std::string & firstAttributeName) const;
  cs::Color4f LoadColor4f(const cs::file::Entry * entry, size_t offset = 0) const;
  cs::Color4f LoadColor4f(const cs::file::Entry * entry, const std::string & firstAttributeName5) const;
};

}