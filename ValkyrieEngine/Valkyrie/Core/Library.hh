#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <string>


struct vkLibraryPriv;

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
  vkLibraryPriv *m_priv;

};