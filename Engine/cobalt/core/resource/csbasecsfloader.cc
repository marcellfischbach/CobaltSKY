
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <tixml/tinyxml.h>


csBaseCSFLoader::csBaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csBaseCSFLoader::~csBaseCSFLoader()
{

}

const csfEntry *csBaseCSFLoader::FindEntry(const csfEntry *parent, const std::string &entryName, const std::string &name) const
{
  if (!parent)
  {
    return 0;
  }

  std::string tagName = parent->GetTagName();
  if (tagName == entryName)
  {
    return parent;
  }

  for (size_t i = 0, in = parent->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *entry = parent->GetChild(i);
    if (entryName != entry->GetTagName())
    {
      continue;
    }
    
    if (name.length() == 0)
    {
      return entry;
    }

    if (entry->HasAttribute("name"))
    {
      continue;
    }
    if (entry->GetAttribute("name") == name)
    {
      return entry;
    }
  }

  return 0;
}

const csfEntry *csBaseCSFLoader::FindEntryByTagName(const csfEntry *parent, const std::string &entryName) const
{
  if (!parent)
  {
    return 0;
  }

  std::string tagName = parent->GetTagName();
  if (tagName == entryName)
  {
    return parent;
  }

  for (size_t i = 0, in = parent->GetNumberOfChildren(); i < in; ++i)
  {
    const csfEntry *entry = parent->GetChild(i);
    if (entryName.length() == 0 || entry->GetTagName() == entryName)
    {
      return entry;
    }
  }

  return 0;
}

bool csBaseCSFLoader::HasLocator(const csfEntry *entry) const
{
  return entry->HasAttribute("locator");
}

csResourceLocator csBaseCSFLoader::GetLocator(const csfEntry *entry) const
{
  return csResourceLocator (entry->GetAttribute("locator"));
}


csResourceLoadingMode csBaseCSFLoader::GetResourceLoadingMode(const csfEntry *entry, csResourceLoadingMode defaultMode, csResourceLoadingMode alterInline) const
{
  csResourceLoadingMode mode = defaultMode;
  if (entry->HasAttribute("resourceMode"))
  {
    std::string modeString = entry->GetAttribute("resourceMode");
    if (modeString == std::string("shared"))
    {
      mode = eRLM_Shared;
    }
    else if (modeString == std::string("instance"))
    {
      mode = eRLM_Instance;
    }
    else if (modeString == std::string("inline"))
    {
      mode = alterInline;
    }
  }
  return mode;
}

bool csBaseCSFLoader::LoadBool(const csfEntry *entry, size_t idx) const
{
  return entry->GetAttribute(idx) == std::string("true");
}

bool csBaseCSFLoader::LoadBool(const csfEntry *entry, const std::string &attributeName) const
{
  size_t idx = entry->GetAttributeIndex(attributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return false;
  }
  return entry->GetAttribute(idx) == std::string("true");
}

float csBaseCSFLoader::LoadFloat(const csfEntry *entry, size_t idx) const
{
  return entry->GetAttributeFloat(idx);
}

float csBaseCSFLoader::LoadFloat(const csfEntry *entry, const std::string &attributeName) const
{
  size_t idx = entry->GetAttributeIndex(attributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return 0.0f;
  }
  return entry->GetAttributeFloat(idx);
}

csVector2f csBaseCSFLoader::LoadVector2f(const csfEntry *entry, size_t idx) const
{
  return csVector2f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1));
}

csVector2f csBaseCSFLoader::LoadVector2f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return csVector2f(0.0f, 0.0f);
  }
  return csVector2f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1));
}

csVector3f csBaseCSFLoader::LoadVector3f(const csfEntry *entry, size_t idx) const
{
  return csVector3f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1),
    entry->GetAttributeFloat(idx+2));
}

csVector3f csBaseCSFLoader::LoadVector3f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return csVector3f(0.0f, 0.0f, 0.0f);
  }
  return csVector3f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1),
    entry->GetAttributeFloat(idx + 2));
}

csVector4f csBaseCSFLoader::LoadVector4f(const csfEntry *entry, size_t idx) const
{
  return csVector4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1),
    entry->GetAttributeFloat(idx+2),
    entry->GetAttributeFloat(idx+3));
}

csVector4f csBaseCSFLoader::LoadVector4f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return csVector4f(0.0f, 0.0f, 0.0f, 0.0f);
  }
  return csVector4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1),
    entry->GetAttributeFloat(idx + 2),
    entry->GetAttributeFloat(idx + 3));
}

csColor4f csBaseCSFLoader::LoadColor4f(const csfEntry *entry, size_t idx) const
{
  return csColor4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1),
    entry->GetAttributeFloat(idx+2),
    entry->GetAttributeFloat(idx+3));
}

csColor4f csBaseCSFLoader::LoadColor4f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return csColor4f(0.0f, 0.0f, 0.0f, 0.0f);
  }
  return csColor4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1),
    entry->GetAttributeFloat(idx + 2),
    entry->GetAttributeFloat(idx + 3));
}


