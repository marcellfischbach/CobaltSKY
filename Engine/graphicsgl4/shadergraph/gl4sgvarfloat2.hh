#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat2.refl.hh>


namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGVarFloat2GL4 : public CS_SUPER(cs::SGVarGL4)
{
  CS_CLASS_GEN;
public:
  SGVarFloat2GL4() : cs::SGVarGL4("vec2") { }
  virtual ~SGVarFloat2GL4() { }
};

}