#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <string>
#include <cobalt/core/cslibrary.refl.hh>


struct csLibraryPriv;

CS_CLASS();
class csLibrary : public CS_SUPER(cs::Object)
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
