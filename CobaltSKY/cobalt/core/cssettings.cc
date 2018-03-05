#include <cobalt/core/cssettings.hh>
#include <stdio.h>
#include <string.h>

csSettings* csSettings::static_instances = 0;

csSettings::csSettings()
{
}

bool csSettings::Initialize(int argc, char** argv)
{
  bool useDefault = true;
  for (int i=1; i<argc; i++)
  {
    if (strcmp (argv[i], "--config") == 0)
    {
      if (i >= argc) return false;
      if (!Initialize(argv[++i]))
      {
        return false;
      }
      useDefault = false;
    }
  }
  return Initialize(".");
}

static std::string strip_path (const std::string &path)
{
  std::string p = path;
  for (int i=(int)p.length()-1; i>=0; --i)
  {
    if (p[i] == '/' || p[i] == '\\')
    {
      p[i] = '\0';
      break;
    }
  }
  return std::string(p.c_str());
}

bool csSettings::Initialize(const std::string &configFileName)
{
  if (!m_file.Parse(std::string(configFileName)))
  {
    return false;
  }
  m_configEntry = m_file.GetEntry("config");
  if (!m_configEntry)
  {
    return false;
  }
  _rootPath = strip_path(configFileName);

  return true;
}



csSettings* csSettings::Get()
{
  if (!csSettings::static_instances)
  {
    csSettings::static_instances = new csSettings(); 
  }
  return csSettings::static_instances;
}

const std::string &csSettings::GetRootPath () const
{
  return _rootPath;
}

bool csSettings::HasEntry(const std::string &entryName) const
{
  return m_configEntry && m_configEntry->GetEntry(entryName);
}

const csfEntry *csSettings::GetEntry(const std::string &entryName) const
{
  return m_configEntry ? m_configEntry->GetEntry(entryName) : 0;
}

bool csSettings::HasValue(const std::string &entryName, size_t attribute) const
{
  const csfEntry *entry = GetEntry(entryName);

  return entry && entry->GetNumberOfAttributes() > attribute;
}


bool csSettings::HasValue(const std::string &entryName, const std::string &attributeName) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName);
}

std::string csSettings::GetStringValue(const std::string &entryName, size_t attribute, const std::string &defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->GetNumberOfAttributes() > attribute ? entry->GetAttribute(attribute) : defaultValue;
}

std::string csSettings::GetStringValue(const std::string &entryName, const std::string &attributeName, const std::string &defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName) ? entry->GetAttribute(attributeName) : defaultValue;
}

int csSettings::GetIntValue(const std::string &entryName, size_t attribute, int defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->GetNumberOfAttributes() > attribute ? entry->GetAttributeInt(attribute) : defaultValue;
}

int csSettings::GetIntValue(const std::string &entryName, const std::string &attributeName, int defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName) ? entry->GetAttributeInt(attributeName) : defaultValue;
}


float csSettings::GetFloatValue(const std::string &entryName, size_t attribute, float defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->GetNumberOfAttributes() > attribute ? entry->GetAttributeFloat(attribute) : defaultValue;
}

float csSettings::GetFloatValue(const std::string &entryName, const std::string &attributeName, float defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName) ? entry->GetAttributeFloat(attributeName) : defaultValue;
}


bool csSettings::GetBoolValue(const std::string &entryName, size_t attribute, bool defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  if (entry && entry->GetNumberOfAttributes() > attribute)
  {
    std::string value = entry->GetAttribute(attribute);
    return value == "true" || value == "on" || value == "yes";
  }
  return defaultValue;
}

bool csSettings::GetBoolValue(const std::string &entryName, const std::string &attributeName, bool defaultValue) const
{
  const csfEntry *entry = GetEntry(entryName);
  if (entry && entry->HasAttribute(attributeName))
  {
    std::string value = entry->GetAttribute(attributeName);
    return value == "true" || value == "on" || value == "yes";
  }
  return defaultValue;
}

