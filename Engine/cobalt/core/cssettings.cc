#include <cobalt/core/cssettings.hh>
#include <stdio.h>
#include <string.h>

cs::Settings* cs::Settings::static_instances = 0;

cs::Settings::Settings()
{
}

bool cs::Settings::Initialize(int argc, char** argv)
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

bool cs::Settings::Initialize(const std::string &configFileName)
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



cs::Settings* cs::Settings::Get()
{
  if (!cs::Settings::static_instances)
  {
    cs::Settings::static_instances = new cs::Settings(); 
  }
  return cs::Settings::static_instances;
}

const std::string &cs::Settings::GetRootPath () const
{
  return _rootPath;
}

bool cs::Settings::HasEntry(const std::string &entryName) const
{
  return m_configEntry && m_configEntry->GetEntry(entryName);
}

const cs::file::Entry *cs::Settings::GetEntry(const std::string &entryName) const
{
  return m_configEntry ? m_configEntry->GetEntry(entryName) : 0;
}

bool cs::Settings::HasValue(const std::string &entryName, size_t attribute) const
{
  const cs::file::Entry *entry = GetEntry(entryName);

  return entry && entry->GetNumberOfAttributes() > attribute;
}


bool cs::Settings::HasValue(const std::string &entryName, const std::string &attributeName) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName);
}

std::string cs::Settings::GetStringValue(const std::string &entryName, size_t attribute, const std::string &defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->GetNumberOfAttributes() > attribute ? entry->GetAttribute(attribute) : defaultValue;
}

std::string cs::Settings::GetStringValue(const std::string &entryName, const std::string &attributeName, const std::string &defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName) ? entry->GetAttribute(attributeName) : defaultValue;
}

int cs::Settings::GetIntValue(const std::string &entryName, size_t attribute, int defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->GetNumberOfAttributes() > attribute ? entry->GetAttributeInt(attribute) : defaultValue;
}

int cs::Settings::GetIntValue(const std::string &entryName, const std::string &attributeName, int defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName) ? entry->GetAttributeInt(attributeName) : defaultValue;
}


float cs::Settings::GetFloatValue(const std::string &entryName, size_t attribute, float defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->GetNumberOfAttributes() > attribute ? entry->GetAttributeFloat(attribute) : defaultValue;
}

float cs::Settings::GetFloatValue(const std::string &entryName, const std::string &attributeName, float defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  return entry && entry->HasAttribute(attributeName) ? entry->GetAttributeFloat(attributeName) : defaultValue;
}


bool cs::Settings::GetBoolValue(const std::string &entryName, size_t attribute, bool defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  if (entry && entry->GetNumberOfAttributes() > attribute)
  {
    std::string value = entry->GetAttribute(attribute);
    return value == "true" || value == "on" || value == "yes";
  }
  return defaultValue;
}

bool cs::Settings::GetBoolValue(const std::string &entryName, const std::string &attributeName, bool defaultValue) const
{
  const cs::file::Entry *entry = GetEntry(entryName);
  if (entry && entry->HasAttribute(attributeName))
  {
    std::string value = entry->GetAttribute(attributeName);
    return value == "true" || value == "on" || value == "yes";
  }
  return defaultValue;
}

