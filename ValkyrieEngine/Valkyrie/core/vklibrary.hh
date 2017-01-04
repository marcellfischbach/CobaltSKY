#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <string>
#include <Valkyrie/core/vklibrary.refl.hh>


struct vkLibraryPriv;

VK_CLASS();
class vkLibrary : public VK_SUPER(vkObject)
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
