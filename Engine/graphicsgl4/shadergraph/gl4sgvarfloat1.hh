#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat1.refl.hh>

namespace cs
{

CS_CLASS()
class CSGRAPHICSGL4_API SGVarFloatGL4 : public CS_SUPER(cs::SGVarGL4)
{
  CS_CLASS_GEN;
public:
  SGVarFloatGL4() : cs::SGVarGL4("float") { }
  virtual ~SGVarFloatGL4() { }
};

}