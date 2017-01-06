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
struct IRenderTarget;
struct IShaderAttribute;
struct ITexture;



VK_INTERFACE()
class VKE_API vkPostProcess : public VK_SUPER(vkObject)
{
  friend class vkPostProcessor;
  VK_CLASS_GEN;
public:
  virtual ~vkPostProcess();

  virtual bool Render(IGraphics *graphics);
  virtual bool Initialize(IGraphics *graphics);

  int BindInput(ITexture *texture, const vkString &inputName = "");
  int BindInput(vkPostProcess *postProcess, int outputIdx, const vkString &inputName = "");
  int BindInput(vkPostProcessOutput originOutput, const vkString &inputName = "");

  void SetOutput(IRenderTarget *output);
  IRenderTarget *GetOutput();

  IShader *GetShader();
  const IShader *GetShader() const;

protected:
  vkPostProcess();

  void SetInputBindingName(int idx, const vkString &name);
  void SetShader(IShader *shader);
  
  bool BindShader(IGraphics *graphics);
  bool BindInputs(IGraphics *graphics);
  bool BindOutput(IGraphics *graphics);

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
    ITexture *m_texture;
    vkPostProcess *m_postProcess;
    int m_postProcessOutput;
    vkPostProcessOutput m_originOutput;

    // shader attributes
    bool m_initialized;
    IShaderAttribute *m_attrInput;
    IShaderAttribute *m_attrInputSize;
    IShaderAttribute *m_attrInputSizeInv;
  };

  std::vector<Input> m_inputs;

  IRenderTarget *m_output;
  vkPostProcessor *m_postProcessor;

  IShader *m_shader;

};


VK_FORCEINLINE IShader *vkPostProcess::GetShader()
{
  return m_shader;
}

