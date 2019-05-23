
#include <cobalt/core/resource/csbasecsfloader.hh>


cs::BaseCSFLoader::BaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::BaseCSFLoader::~BaseCSFLoader()
{

}

const csfEntry *cs::BaseCSFLoader::FindEntry(const csfEntry *parent, const std::string &entryName, const std::string &name) const
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

const csfEntry *cs::BaseCSFLoader::FindEntryByTagName(const csfEntry *parent, const std::string &entryName) const
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

bool cs::BaseCSFLoader::HasLocator(const csfEntry *entry) const
{
  return entry->HasAttribute("locator");
}

cs::ResourceLocator cs::BaseCSFLoader::GetLocator(const csfEntry *entry) const
{
  return cs::ResourceLocator (entry->GetAttribute("locator"));
}


cs::eResourceLoadingMode cs::BaseCSFLoader::GetResourceLoadingMode(const csfEntry *entry, cs::eResourceLoadingMode defaultMode, cs::eResourceLoadingMode alterInline) const
{
  cs::eResourceLoadingMode mode = defaultMode;
  if (entry->HasAttribute("resourceMode"))
  {
    std::string modeString = entry->GetAttribute("resourceMode");
    if (modeString == std::string("shared"))
    {
      mode = cs::eRLM_Shared;
    }
    else if (modeString == std::string("instance"))
    {
      mode = cs::eRLM_Instance;
    }
    else if (modeString == std::string("inline"))
    {
      mode = alterInline;
    }
  }
  return mode;
}

bool cs::BaseCSFLoader::LoadBool(const csfEntry *entry, size_t idx) const
{
  return entry->GetAttribute(idx) == std::string("true");
}

bool cs::BaseCSFLoader::LoadBool(const csfEntry *entry, const std::string &attributeName) const
{
  size_t idx = entry->GetAttributeIndex(attributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return false;
  }
  return entry->GetAttribute(idx) == std::string("true");
}

float cs::BaseCSFLoader::LoadFloat(const csfEntry *entry, size_t idx) const
{
  return entry->GetAttributeFloat(idx);
}

float cs::BaseCSFLoader::LoadFloat(const csfEntry *entry, const std::string &attributeName) const
{
  size_t idx = entry->GetAttributeIndex(attributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return 0.0f;
  }
  return entry->GetAttributeFloat(idx);
}

cs::Vector2f cs::BaseCSFLoader::LoadVector2f(const csfEntry *entry, size_t idx) const
{
  return cs::Vector2f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1));
}

cs::Vector2f cs::BaseCSFLoader::LoadVector2f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return cs::Vector2f(0.0f, 0.0f);
  }
  return cs::Vector2f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1));
}

cs::Vector3f cs::BaseCSFLoader::LoadVector3f(const csfEntry *entry, size_t idx) const
{
  return cs::Vector3f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1),
    entry->GetAttributeFloat(idx+2));
}

cs::Vector3f cs::BaseCSFLoader::LoadVector3f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return cs::Vector3f(0.0f, 0.0f, 0.0f);
  }
  return cs::Vector3f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1),
    entry->GetAttributeFloat(idx + 2));
}

cs::Vector4f cs::BaseCSFLoader::LoadVector4f(const csfEntry *entry, size_t idx) const
{
  return cs::Vector4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1),
    entry->GetAttributeFloat(idx+2),
    entry->GetAttributeFloat(idx+3));
}

cs::Vector4f cs::BaseCSFLoader::LoadVector4f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return cs::Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  }
  return cs::Vector4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1),
    entry->GetAttributeFloat(idx + 2),
    entry->GetAttributeFloat(idx + 3));
}

cs::Color4f cs::BaseCSFLoader::LoadColor4f(const csfEntry *entry, size_t idx) const
{
  return cs::Color4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx+1),
    entry->GetAttributeFloat(idx+2),
    entry->GetAttributeFloat(idx+3));
}

cs::Color4f cs::BaseCSFLoader::LoadColor4f(const csfEntry *entry, const std::string &firstAttributeName) const
{
  size_t idx = entry->GetAttributeIndex(firstAttributeName);
  if (idx == csfEntry::InvalidIndex)
  {
    return cs::Color4f(0.0f, 0.0f, 0.0f, 0.0f);
  }
  return cs::Color4f(
    entry->GetAttributeFloat(idx),
    entry->GetAttributeFloat(idx + 1),
    entry->GetAttributeFloat(idx + 2),
    entry->GetAttributeFloat(idx + 3));
}


