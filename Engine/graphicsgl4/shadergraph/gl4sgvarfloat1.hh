#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat1.refl.hh>


CS_CLASS()
class CSGRAPHICSGL4_API csSGVarFloatGL4 : public CS_SUPER(csSGVarGL4)
{
  CS_CLASS_GEN;
public:
  csSGVarFloatGL4() : csSGVarGL4("float") { }
  virtual ~csSGVarFloatGL4() { }
};