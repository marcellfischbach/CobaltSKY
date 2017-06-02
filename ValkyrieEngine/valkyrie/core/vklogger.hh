

#pragma once

#include <valkyrie/vkexport.hh>
#include <string>

#ifdef VK_WIN32
#include <windows.h>
#endif

class VKE_API vkLogger
{
public:
  vkLogger(const std::string &name = std::string(""));

  void Debug(const std::string &debug);
  void Info(const std::string &info);
  void Warn(const std::string &warning);
  void Error(const std::string &error);
  void Fatal(const std::string &fatal);

private:
  bool m_debug = true;
  bool m_info = true;
  bool m_warn = true;
  bool m_error = true;
  bool m_fatal = true;

  std::string m_name;
  std::string m_outputName;
#ifdef VK_WIN32
  HANDLE m_consoleHandle;
#endif
};

