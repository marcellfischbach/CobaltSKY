#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <vector>
#include <map>

#include <Valkyrie/Graphics/PostProcessing.refl.hh>

class vkPostProcess;
struct IRenderTarget;
struct ITexture;

VK_CLASS()
class VKE_API vkPostProcessor : public vkObject
{
  VK_CLASS_GEN;
public:
  vkPostProcessor();
  virtual ~vkPostProcessor();

  void SetInput(const vkString &inputName, ITexture *texture);
  const ITexture *GetInput(const vkString &inputName) const;
  ITexture *GetInput(const vkString &inputName);

  void SetFinalProcess(vkPostProcess *postProcess);
  void BuildPostProcessing();

private:
  vkPostProcess *m_finalProcess;
  std::vector<vkPostProcess*> m_processes;
  std::map<vkString, ITexture*> m_inputs;
};


VK_INTERFACE()
class VKE_API vkPostProcess : public vkObject
{
  friend class vkPostProcessor;
  VK_CLASS_GEN;
public:
  virtual ~vkPostProcess();

  virtual void Render(IGraphics *graphics) = 0;

  int BindInput(ITexture *texture, const vkString &inputName = "");
  int BindInput(vkPostProcess *postProcess, int outputIdx, const vkString &inputName = "");
  int BindInput(const vkString &outputName, const vkString &inputName = "");

  void SetOutput(IRenderTarget *output);

  IShader *GetShader();
  const IShader *GetShader() const;

protected:
  vkPostProcess();

  void SetInputBindingName(int idx, const vkString &name);

  void SetShader(IShader *shader);
  

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
    vkString m_originOutput;
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

  virtual void Render(IGraphics *graphics);

  void SetShader(IShader *shader);


};


VK_FORCEINLINE IShader *vkPostProcess::GetShader()
{
  return m_shader;
}

