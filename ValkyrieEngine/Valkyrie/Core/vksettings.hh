

#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Core/vkstring.hh>
#include <tixml/tinyxml.h>


class VKE_API vkSettings
{

public:

  static bool Initialize (int argc, char** argv);
  static bool Initialize (const char* configFileName);
  static vkSettings* Get ();

  const char* GetRootPath () const;

  bool HasGroup (const char* group);
  bool HasValue (const char* group, const char* entry = 0);
  TiXmlElement *GetGroup (const char* group);
  const char* GetStringValue (const char* group, const char* entry = 0);
  int GetIntValue (const char* group, const char* entry = 0, int defaultValue = 0);
  float GetFloatValue (const char* group, const char* entry = 0, float defaultValue = 0.0f);
  bool GetBoolValue (const char* group, const char* entry = 0, bool defaultValue = false);

protected:
  vkSettings();

private:
  TiXmlDocument     *m_document;
  static vkSettings*  static_instances;
  std::string         _rootPath;

  TiXmlElement *GetElement(const char* group, const char* entry);

};

