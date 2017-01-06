#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgvar.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgvarfloat3.refl.hh>


VK_CLASS()
class VKGRAPHICSGL4_API vkSGVarFloat3GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat3GL4() : vkSGVarGL4("vec3") { }
  virtual ~vkSGVarFloat3GL4() { }
};