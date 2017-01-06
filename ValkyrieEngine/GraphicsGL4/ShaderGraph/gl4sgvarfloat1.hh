#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgvar.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgvarfloat1.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarFloatGL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloatGL4() : vkSGVarGL4("float") { }
  virtual ~vkSGVarFloatGL4() { }
};