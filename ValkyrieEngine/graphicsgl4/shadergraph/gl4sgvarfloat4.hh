#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat4.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarFloat4GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat4GL4() : vkSGVarGL4("vec4") { }
  virtual ~vkSGVarFloat4GL4() { }
};