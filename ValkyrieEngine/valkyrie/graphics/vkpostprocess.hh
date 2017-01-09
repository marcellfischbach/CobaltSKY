#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/vkpostprocessoutput.hh>
#include <vector>
#include <map>
#include <set>
#include <valkyrie/graphics/vkpostprocess.refl.hh>


class vkPostProcess;
struct iRenderTarget;
struct iShaderAttribute;
struct iTexture;



VK_INTERFACE()
class VKE_API vkPostProcess : public VK_SUPER(vkObject)
{
  friend class vkPostProcessor;
  VK_CLASS_GEN;
public:
  virtual ~vkPostProcess();

  virtual bool Render(iGraphics *graphics);
  virtual bool Initialize(iGraphics *graphics);

  int BindInput(iTexture *texture, const vkString &inputName = "");
  int BindInput(vkPostProcess *postProcess, int outputIdx, const vkString &inputName = "");
  int BindInput(vkPostProcessOutput originOutput, const vkString &inputName = "");

  void SetOutput(iRenderTarget *output);
  iRenderTarget *GetOutput();

  iShader *GetShader();
  const iShader *GetShader() const;

protected:
  vkPostProcess();

  void SetInputBindingName(int idx, const vkString &name);
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
    vkString m_inputName;
    iTexture *m_texture;
    vkPostProcess *m_postProcess;
    int m_postProcessOutput;
    vkPostProcessOutput m_originOutput;

    // shader attributes
    bool m_initialized;
    iShaderAttribute *m_attrInput;
    iShaderAttribute *m_attrInputSize;
    iShaderAttribute *m_attrInputSizeInv;
  };

  std::vector<Input> m_inputs;

  iRenderTarget *m_output;
  vkPostProcessor *m_postProcessor;

  iShader *m_shader;

};


VK_FORCEINLINE iShader *vkPostProcess::GetShader()
{
  return m_shader;
}

