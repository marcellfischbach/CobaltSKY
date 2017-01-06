#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat1.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarFloatGL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloatGL4() : vkSGVarGL4("float") { }
  virtual ~vkSGVarFloatGL4() { }
};