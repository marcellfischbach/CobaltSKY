
#include <cobalt/core/resource/csbasexmlloader.hh>
#include <tixml/tinyxml.h>


csBaseXMLLoader::csBaseXMLLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csBaseXMLLoader::~csBaseXMLLoader()
{

}

TiXmlElement *csBaseXMLLoader::FindElement(TiXmlElement *root, const std::string &elementName, const std::string &name) const
{
  if (!root)
  {
    return 0;
  }

  std::string tagName(root->Value());
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
    std::string thisName(c_attr);
    if (thisName == name)
    {
      return element;
    }
  }

  return 0;
}

TiXmlElement *csBaseXMLLoader::FindElementByTagName(TiXmlElement *root, const std::string &elementName) const
{
  if (!root)
  {
    return 0;
  }

  std::string tagName(root->Value());
  if (tagName == elementName)
  {
    return root;
  }

  for (TiXmlElement *element = root->FirstChildElement();
       element;
       element = element->NextSiblingElement())
  {
    if (elementName.length() == 0 || std::string(element->Value()) == elementName)
    {
      return element;
    }
  }

  return 0;
}


csResourceLoadingMode csBaseXMLLoader::GetResourceLoadingMode(TiXmlElement *element, csResourceLoadingMode defaultMode, csResourceLoadingMode alterInline) const
{
  csResourceLoadingMode mode = defaultMode;
  if (element->Attribute("resourceMode"))
  {
    std::string modeString(element->Attribute("resourceMode"));
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
  if (!element->GetText())
  {
    // no references should be inline
    mode = alterInline;
  }
  return mode;
}

bool csBaseXMLLoader::LoadBool(const char *str) const
{
  return str == std::string("true");
}

float csBaseXMLLoader::LoadFloat(const char *str) const
{
  return (float)atof(str);
}


csVector2f csBaseXMLLoader::LoadVector2f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';
#ifdef CS_WIN32
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

  return csVector2f(x, y);
}

csVector3f csBaseXMLLoader::LoadVector3f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef CS_WIN32
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

  return csVector3f(x, y, z);
}

csVector4f csBaseXMLLoader::LoadVector4f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef CS_WIN32
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

  return csVector4f(x, y, z, w);
}

csColor4f csBaseXMLLoader::LoadColor4f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef CS_WIN32
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

  return csColor4f(r, g, b, a);
}
