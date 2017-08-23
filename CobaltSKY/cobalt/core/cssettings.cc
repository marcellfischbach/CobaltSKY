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
  if (!m_ini.Import(std::string(configFileName)))
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

std::vector<std::string> csSettings::FindSubGroupNames(const std::string &groupSubName) const
{
  return m_ini.FindSubGroupNames(groupSubName);
}

bool csSettings::HasGroup (const std::string &group)
{
  return m_ini.HasGroup(group);
}

bool csSettings::HasValue(const std::string &entry)
{
  return m_ini.HasEntry(entry);
}

std::string csSettings::GetStringValue(const std::string &entry)
{
  return m_ini.GetValue(entry);
}

int csSettings::GetIntValue(const std::string &entry, int defaultValue)
{
  return m_ini.GetIntValue(entry, defaultValue);
}

float csSettings::GetFloatValue(const std::string &entry, float defaultValue)
{
  return m_ini.GetFloatValue(entry, defaultValue);
}

bool csSettings::GetBoolValue(const std::string &entry, bool defaultValue)
{
  return m_ini.GetBoolValue(entry, defaultValue);
}

bool csSettings::HasValue(const std::string &group, const std::string &entry)
{
  return m_ini.HasEntry(group, entry);
}

std::string csSettings::GetStringValue(const std::string &group, const std::string &entry)
{
  return m_ini.GetValue(group, entry);
}

int csSettings::GetIntValue(const std::string &group, const std::string &entry, int defaultValue)
{
  return m_ini.GetIntValue(group, entry, defaultValue);
}

float csSettings::GetFloatValue(const std::string &group, const std::string &entry, float defaultValue)
{
  return m_ini.GetFloatValue(group, entry, defaultValue);
}

bool csSettings::GetBoolValue(const std::string &group, const std::string &entry, bool defaultValue)
{
  return m_ini.GetBoolValue(group, entry, defaultValue);
}
