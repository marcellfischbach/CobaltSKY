#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat3.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGVarFloat3GL4 : public CS_SUPER(csSGVarGL4)
{
  CS_CLASS_GEN;
public:
  csSGVarFloat3GL4() : csSGVarGL4("vec3") { }
  virtual ~csSGVarFloat3GL4() { }
};