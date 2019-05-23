#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/cspostprocess.hh>

#include <cobalt/graphics/csgenericshaderpostprocess.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API GenericShaderPostProcess : public CS_SUPER(cs::PostProcess)
{
  CS_CLASS_GEN;
public:
  GenericShaderPostProcess();
  virtual ~GenericShaderPostProcess();

  virtual bool Render(cs::iGraphics * graphics);

  void SetShader(cs::iShader * shader);


};

}

