#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <string>
#include <cobalt/core/cslibrary.refl.hh>


struct csLibraryPriv;

CS_CLASS();
class csLibrary : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csLibrary();
  virtual ~csLibrary();

  bool Open(const std::string &libName);
  bool IsOpen() const;
  void Close();

  void *GetProcAddress(const std::string &procName);

private:
  csLibraryPriv *m_priv;

};