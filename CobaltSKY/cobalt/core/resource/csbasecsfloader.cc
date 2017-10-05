
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

bool csBaseCSFLoader::LoadBool(const csfEntry *entry) const
{
  return entry->GetAttribute() == std::string("true");
}

float csBaseCSFLoader::LoadFloat(const csfEntry *entry) const
{
  return entry->GetAttributeFloat();
}


csVector2f csBaseCSFLoader::LoadVector2f(const csfEntry *entry) const
{
  return csVector2f(
    entry->GetAttributeFloat(0),
    entry->GetAttributeFloat(1));
}

csVector3f csBaseCSFLoader::LoadVector3f(const csfEntry *entry) const
{
  return csVector3f(
    entry->GetAttributeFloat(0),
    entry->GetAttributeFloat(1),
    entry->GetAttributeFloat(2));
}

csVector4f csBaseCSFLoader::LoadVector4f(const csfEntry *entry) const
{
  return csVector4f(
    entry->GetAttributeFloat(0),
    entry->GetAttributeFloat(1),
    entry->GetAttributeFloat(2),
    entry->GetAttributeFloat(3));
}

csColor4f csBaseCSFLoader::LoadColor4f(const csfEntry *entry) const
{
  return csColor4f(
    entry->GetAttributeFloat(0),
    entry->GetAttributeFloat(1),
    entry->GetAttributeFloat(2),
    entry->GetAttributeFloat(3));
}
