#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat4.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGVarFloat4GL4 : public CS_SUPER(csSGVarGL4)
{
  CS_CLASS_GEN;
public:
  csSGVarFloat4GL4() : csSGVarGL4("vec4") { }
  virtual ~csSGVarFloat4GL4() { }
};