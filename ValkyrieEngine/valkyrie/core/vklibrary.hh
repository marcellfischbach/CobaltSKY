#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <string>
#include <valkyrie/core/vklibrary.refl.hh>


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
