#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat2.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarFloat2GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat2GL4() : vkSGVarGL4("vec2") { }
  virtual ~vkSGVarFloat2GL4() { }
};