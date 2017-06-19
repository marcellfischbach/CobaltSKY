#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat2.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGVarFloat2GL4 : public CS_SUPER(csSGVarGL4)
{
  CS_CLASS_GEN;
public:
  csSGVarFloat2GL4() : csSGVarGL4("vec2") { }
  virtual ~csSGVarFloat2GL4() { }
};