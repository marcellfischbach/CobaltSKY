#include <Valkyrie/Core/Settings.hh>
#include <stdio.h>
#include <string.h>

csSettings* csSettings::static_instances = 0;

csSettings::csSettings()
{
}

bool csSettings::Initialize(int argc, char** argv)
{
  for (int i=1; i<argc; i++)
  {
    if (strcmp (argv[i], "--config") == 0)
    {
      if (i >= argc) return false;
      return Initialize(argv[i+1]);
    }
  }
  return Initialize(".");
}

static vkString strip_path (const vkString &path)
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

bool csSettings::Initialize(const char *configFileName)
{
  if (csSettings::static_instances)
  {
    delete csSettings::static_instances;
    csSettings::static_instances = 0;
  }

  csSettings* settings = new csSettings ();
  settings->m_document = new TiXmlDocument();
  if (!settings->m_document->LoadFile(configFileName))
  {
    printf("Unable to open config file: %s\n", configFileName);
    delete settings;
    return false;
  }
  settings->_rootPath = strip_path(configFileName);

  csSettings::static_instances = settings;
  return true;
}



csSettings* csSettings::Get()
{
  return csSettings::static_instances;
}

const char* csSettings::GetRootPath () const
{
  return _rootPath.c_str();
}

TiXmlElement *csSettings::GetElement(const char *group, const char *entry)
{
  TiXmlElement *groupElement = m_document->RootElement()->FirstChildElement(group);
  if (!groupElement)
  {
    return 0;
  }

  return groupElement->FirstChildElement(entry);
}

bool csSettings::HasGroup (const char* group)
{
  TiXmlElement *groupElement = m_document->RootElement()->FirstChildElement(group);
  return groupElement != 0;
}

bool csSettings::HasValue(const char *group, const char *entry)
{
  TiXmlElement *groupElement = m_document->RootElement()->FirstChildElement(group);
  if (!groupElement)
  {
    return false;
  }

  return groupElement->FirstChildElement(entry) != 0;
}

TiXmlElement *csSettings::GetGroup (const char* group)
{
  return m_document->RootElement()->FirstChildElement(group);
}

const char* csSettings::GetStringValue(const char *group, const char *entry)
{
  TiXmlElement *element = GetElement(group, entry);
  if (element)
  {
    return element->V
  }
  return 0;
}


int csSettings::GetIntValue(const char *group, const char *entry, int defaultValue)
{
  csiElement *element = GetElement(group, entry);
  if (element)
  {
    return element->GetInt();
  }
  return defaultValue;
}

float csSettings::GetFloatValue(const char *group, const char *entry, float defaultValue)
{
  csiElement *element = GetElement(group, entry);
  if (element)
  {
    return element->GetFloat();
  }
  return defaultValue;
}


bool csSettings::GetBoolValue(const char *group, const char *entry, bool defaultValue)
{
  csiElement *element = GetElement(group, entry);
  if (element)
  {
    return element->GetBool();
  }
  return defaultValue;
}
