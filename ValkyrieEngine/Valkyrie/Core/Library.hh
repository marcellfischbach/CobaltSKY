#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <string>

#include <Valkyrie/Core/Library.refl.hh>
#ifdef WIN32
#include <Windows.h>
#endif

VK_CLASS();
class vkLibrary : public vkObject
{
  VK_CLASS_GEN;
public:
  vkLibrary();
  virtual ~vkLibrary();

  bool Open(const std::string &libName);
  bool IsOpen() const;
  void Close();

  void *GetProcAddress(const std::string &procName);

private:
#ifdef WIN32
  HMODULE m_libModule;
#endif

};