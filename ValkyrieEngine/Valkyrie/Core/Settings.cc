#include <Valkyrie/Core/Settings.hh>
#include <stdio.h>
#include <string.h>

vkSettings* vkSettings::static_instances = 0;

vkSettings::vkSettings()
{
}

bool vkSettings::Initialize(int argc, char** argv)
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

bool vkSettings::Initialize(const char *configFileName)
{
  if (vkSettings::static_instances)
  {
    delete vkSettings::static_instances;
    vkSettings::static_instances = 0;
  }

  vkSettings* settings = new vkSettings ();
  settings->m_document = new TiXmlDocument();
  if (!settings->m_document->LoadFile(configFileName))
  {
    printf("Unable to open config file: %s\n", configFileName);
    delete settings;
    return false;
  }
  settings->_rootPath = strip_path(configFileName);

  vkSettings::static_instances = settings;
  return true;
}



vkSettings* vkSettings::Get()
{
  return vkSettings::static_instances;
}

const char* vkSettings::GetRootPath () const
{
  return _rootPath.c_str();
}

TiXmlElement *vkSettings::GetElement(const char *group, const char *entry)
{
  TiXmlElement *groupElement = m_document->RootElement()->FirstChildElement(group);
  if (!groupElement)
  {
    return 0;
  }

  return groupElement->FirstChildElement(entry);
}

bool vkSettings::HasGroup (const char* group)
{
  TiXmlElement *groupElement = m_document->RootElement()->FirstChildElement(group);
  return groupElement != 0;
}

bool vkSettings::HasValue(const char *group, const char *entry)
{
  TiXmlElement *groupElement = m_document->RootElement()->FirstChildElement(group);
  if (!groupElement)
  {
    return false;
  }

  return groupElement->FirstChildElement(entry) != 0;
}

TiXmlElement *vkSettings::GetGroup (const char* group)
{
  return m_document->RootElement()->FirstChildElement(group);
}

const char* vkSettings::GetStringValue(const char *group, const char *entry)
{
  TiXmlElement *element = GetElement(group, entry);
  if (element)
  {
    return element->GetText();
  }
  return 0;
}


int vkSettings::GetIntValue(const char *group, const char *entry, int defaultValue)
{
  const char *text = GetStringValue(group, entry);
  if (!text)
  {
    return defaultValue;
  }

  return atoi(text);
}

float vkSettings::GetFloatValue(const char *group, const char *entry, float defaultValue)
{
  const char *text = GetStringValue(group, entry);
  if (!text)
  {
    return defaultValue;
  }

  return (float)atof(text);
}


bool vkSettings::GetBoolValue(const char *group, const char *entry, bool defaultValue)
{
  const char *text = GetStringValue(group, entry);
  if (!text)
  {
    return defaultValue;
  }
  vkString tStr(text);
  return tStr == vkString("true");
}
