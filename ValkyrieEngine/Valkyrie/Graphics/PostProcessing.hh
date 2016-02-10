#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <vector>
#include <map>
#include <set>

#include <Valkyrie/Graphics/PostProcessing.refl.hh>

class vkPostProcess;
struct IRenderTarget;
struct IShaderAttribute;
struct ITexture;

VK_CLASS()
class VKE_API vkPostProcessor : public vkObject
{
  VK_CLASS_GEN;
public:
  enum OriginOutput
  {
    eOO_GBuffer_Depth,
    eOO_GBuffer_DiffuseRoughness,
    eOO_GBuffer_NormalLightMode,
    eOO_GBuffer_EmissiveMetallic,
    eOO_GBuffer_SSSSpec,
    eOO_FinalTarget_Color,
    eOO_FinalTarget_Depth,
    eOO_COUNT
  };
public:
  vkPostProcessor();
  virtual ~vkPostProcessor();

  void SetInput(OriginOutput originOutput, ITexture *texture);
  const ITexture *GetInput(OriginOutput originOutput) const;
  ITexture *GetInput(OriginOutput originOutput);

  void SetFinalProcess(vkPostProcess *postProcess);
  bool BuildPostProcessing(IGraphics *graphics);

  void Render(IGraphics *graphics);

  IRenderTarget *GetOutput();


private:
  void BuildSet(vkPostProcess *process, std::set<vkPostProcess*> &processes);
  vkPostProcess *m_finalProcess;
  std::vector<vkPostProcess*> m_processes;
  ITexture *m_originInputs[eOO_COUNT];
};


VK_INTERFACE()
class VKE_API vkPostProcess : public vkObject
{
  friend class vkPostProcessor;
  VK_CLASS_GEN;
public:
  virtual ~vkPostProcess();

  virtual bool Render(IGraphics *graphics);
  virtual bool Initialize(IGraphics *graphics);

  int BindInput(ITexture *texture, const vkString &inputName = "");
  int BindInput(vkPostProcess *postProcess, int outputIdx, const vkString &inputName = "");
  int BindInput(vkPostProcessor::OriginOutput originOutput, const vkString &inputName = "");

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
    vkPostProcessor::OriginOutput m_originOutput;

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


VK_CLASS()
class VKE_API vkGenericShaderPostProcess : public vkPostProcess
{
  VK_CLASS_GEN;
public:
  vkGenericShaderPostProcess();
  virtual ~vkGenericShaderPostProcess();

  virtual bool Render(IGraphics *graphics);

  void SetShader(IShader *shader);


};


VK_FORCEINLINE IShader *vkPostProcess::GetShader()
{
  return m_shader;
}

