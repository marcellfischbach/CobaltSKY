
#include <valkyrie/core/resource/vkbasexmlloader.hh>
#include <tixml/tinyxml.h>


vkBaseXMLLoader::vkBaseXMLLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkBaseXMLLoader::~vkBaseXMLLoader()
{

}

TiXmlElement *vkBaseXMLLoader::FindElement(TiXmlElement *root, const vkString &elementName, const vkString &name) const
{
  if (!root)
  {
    return 0;
  }

  vkString tagName(root->Value());
  if (tagName == elementName)
  {
    return root;
  }

  for (TiXmlElement *element = root->FirstChildElement(elementName.c_str());
       element;
       element = element->NextSiblingElement(elementName.c_str()))
  {
    if (name.length() == 0)
    {
      return element;
    }

    const char *c_attr = element->Attribute("name");
    if (!c_attr)
    {
      continue;
    }
    vkString thisName(c_attr);
    if (thisName == name)
    {
      return element;
    }
  }

  return 0;
}

TiXmlElement *vkBaseXMLLoader::FindElementByTagName(TiXmlElement *root, const vkString &elementName) const
{
  if (!root)
  {
    return 0;
  }

  vkString tagName(root->Value());
  if (tagName == elementName)
  {
    return root;
  }

  for (TiXmlElement *element = root->FirstChildElement();
       element;
       element = element->NextSiblingElement())
  {
    if (elementName.length() == 0 || vkString(element->Value()) == elementName)
    {
      return element;
    }
  }

  return 0;
}


vkResourceLoadingMode vkBaseXMLLoader::GetResourceLoadingMode(TiXmlElement *element, vkResourceLoadingMode defaultMode, vkResourceLoadingMode alterInline) const
{
  vkResourceLoadingMode mode = defaultMode;
  if (element->Attribute("resourceMode"))
  {
    vkString modeString(element->Attribute("resourceMode"));
    if (modeString == vkString("shared"))
    {
      mode = eRLM_Shared;
    }
    else if (modeString == vkString("instance"))
    {
      mode = eRLM_Instance;
    }
    else if (modeString == vkString("inline"))
    {
      mode = alterInline;
    }
  }
  if (!element->GetText())
  {
    // no references should be inline
    mode = alterInline;
  }
  return mode;
}

bool vkBaseXMLLoader::LoadBool(const char *str) const
{
  return str == std::string("true");
}

float vkBaseXMLLoader::LoadFloat(const char *str) const
{
  return (float)atof(str);
}


vkVector2f vkBaseXMLLoader::LoadVector2f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';
#ifdef VK_WIN32
  char *ctx;
  char *sx = strtok_s(s, " ;,", &ctx);
  char *sy = strtok_s(0, " ;,", &ctx);
#else
  char *sx = strtok(s, " ;,");
  char *sy = strtok(0, " ;,");
#endif

  float x = (float)atof(sx);
  float y = (float)atof(sy);
  delete[] s;

  return vkVector2f(x, y);
}

vkVector3f vkBaseXMLLoader::LoadVector3f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef VK_WIN32
  char *ctx;
  char *sx = strtok_s(s, " ;,", &ctx);
  char *sy = strtok_s(0, " ;,", &ctx);
  char *sz = strtok_s(0, " ;,", &ctx);
#else
  char *sx = strtok(s, " ;,");
  char *sy = strtok(0, " ;,");
  char *sz = strtok(0, " ;,");
#endif

  float x = (float)atof(sx);
  float y = (float)atof(sy);
  float z = (float)atof(sz);
  delete[] s;

  return vkVector3f(x, y, z);
}

vkVector4f vkBaseXMLLoader::LoadVector4f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef VK_WIN32
  char *ctx;
  char *sx = strtok_s(s, " ;,", &ctx);
  char *sy = strtok_s(0, " ;,", &ctx);
  char *sz = strtok_s(0, " ;,", &ctx);
  char *sw = strtok_s(0, " ;,", &ctx);
#else
  char *sx = strtok(s, " ;,");
  char *sy = strtok(0, " ;,");
  char *sz = strtok(0, " ;,");
  char *sw = strtok(0, " ;,");
#endif

  float x = (float)atof(sx);
  float y = (float)atof(sy);
  float z = (float)atof(sz);
  float w = (float)atof(sw);
  delete[] s;

  return vkVector4f(x, y, z, w);
}

vkColor4f vkBaseXMLLoader::LoadColor4f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef VK_WIN32
  char *ctx;
  char *sr = strtok_s(s, " ;,", &ctx);
  char *sg = strtok_s(0, " ;,", &ctx);
  char *sb = strtok_s(0, " ;,", &ctx);
  char *sa = strtok_s(0, " ;,", &ctx);
#else
  char *sr = strtok(s, " ;,");
  char *sg = strtok(0, " ;,");
  char *sb = strtok(0, " ;,");
  char *sa = strtok(0, " ;,");
#endif

  float r = (float)atof(sr);
  float g = (float)atof(sg);
  float b = (float)atof(sb);
  float a = (float)atof(sa);
  delete[] s;

  return vkColor4f(r, g, b, a);
}
