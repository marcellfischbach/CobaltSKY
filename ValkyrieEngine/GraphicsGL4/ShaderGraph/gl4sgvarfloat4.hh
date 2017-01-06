#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgvar.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgvarfloat4.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarFloat4GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat4GL4() : vkSGVarGL4("vec4") { }
  virtual ~vkSGVarFloat4GL4() { }
};