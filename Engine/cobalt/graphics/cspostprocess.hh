#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cobalt/graphics/cspostprocess.refl.hh>


class csPostProcess;
class csPostProcessor;
struct iRenderTarget;
struct iShaderAttribute;
class csTextureWrapper;



CS_INTERFACE()
class CSE_API csPostProcess : public CS_SUPER(csObject)
{
  friend class csPostProcessor;
  CS_CLASS_GEN;
public:
  virtual ~csPostProcess();

  virtual bool Render(iGraphics *graphics);
  virtual bool Initialize(iGraphics *graphics);

  int BindInput(csTextureWrapper *texture, const std::string &inputName = "");
  int BindInput(csPostProcess *postProcess, int outputIdx, const std::string &inputName = "");
  int BindInput(csPostProcessOutput originOutput, const std::string &inputName = "");

  void SetOutput(iRenderTarget *output);
  iRenderTarget *GetOutput();

  iShader *GetShader();
  const iShader *GetShader() const;

protected:
  csPostProcess();

  void SetInputBindingName(int idx, const std::string &name);
  void SetShader(iShader *shader);
  
  bool BindShader(iGraphics *graphics);
  bool BindInputs(iGraphics *graphics);
  bool BindOutput(iGraphics *graphics);

private:
  enum InputSource
  {
    eIS_Texture,
    eIS_PostProcess,
    eIS_Origin,
  };

  struct Input
  {
    InputSource m_inputSource;
    std::string m_inputName;
    csTextureWrapper *m_texture;
    csPostProcess *m_postProcess;
    int m_postProcessOutput;
    csPostProcessOutput m_originOutput;

    // shader attributes
    bool m_initialized;
    iShaderAttribute *m_attrInput;
    iShaderAttribute *m_attrInputSize;
    iShaderAttribute *m_attrInputSizeInv;
  };

  std::vector<Input> m_inputs;

  iRenderTarget *m_output;
  csPostProcessor *m_postProcessor;

  iShader *m_shader;

};


CS_FORCEINLINE iShader *csPostProcess::GetShader()
{
  return m_shader;
}

