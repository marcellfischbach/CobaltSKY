#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <vector>
#include <map>

class vkPostProcess;
struct IRenderTarget;
struct ITexture;

VK_CLASS()
class vkPostProcessor : public vkObject
{
  VK_CLASS_GEN;
public:
  vkPostProcessor();
  virtual ~vkPostProcessor();

  void SetInput(const vkString &inputName, ITexture *texture);
  ITexture *GetInput(const vkString &inputName);

  void SetFinalProcess(vkPostProcess *postProcess);

private:
  vkPostProcess *m_finalProcess;
  std::vector<vkPostProcess*> m_processes;
  std::map<vkString, ITexture*> m_inputs;
};


VK_INTERFACE()
class vkPostProcess : public vkObject
{
  friend class vkPostProcessor;
  VK_CLASS_GEN;
public:
  virtual ~vkPostProcess();

protected:
  vkPostProcess();

  void SetInputBindingName(int idx, const vkString &name);

  int BindInput(ITexture *texture, const vkString &inputName = "");
  int BindInput(vkPostProcess *postProcess, int outputIdx, const vkString &inputName = "");
  int BindInput(const vkString &inputName);

private:
  enum InputSource
  {
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
    vkString m_originInput;
  };

  std::vector<Input> m_inputs;

  IRenderTarget *m_output;
  vkPostProcessor *m_postProcessor;
};


