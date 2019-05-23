#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <string>
#include <cobalt/core/cslibrary.refl.hh>

namespace cs
{

struct LibraryPriv;

CS_CLASS();
class Library : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  Library();
  virtual ~Library();

  bool Open(const std::string & libName);
  bool IsOpen() const;
  void Close();

  void* GetProcAddress(const std::string & procName);

private:
  cs::LibraryPriv* m_priv;

};

}