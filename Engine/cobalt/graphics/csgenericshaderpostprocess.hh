#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/cspostprocess.hh>

#include <cobalt/graphics/csgenericshaderpostprocess.refl.hh>

CS_CLASS()
class CSE_API csGenericShaderPostProcess : public CS_SUPER(csPostProcess)
{
  CS_CLASS_GEN;
public:
  csGenericShaderPostProcess();
  virtual ~csGenericShaderPostProcess();

  virtual bool Render(iGraphics *graphics);

  void SetShader(iShader *shader);


};
