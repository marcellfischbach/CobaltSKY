#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/vkpostprocessoutput.hh>
#include <set>

class vkPostProcess;
struct IGraphics;
struct IRenderTarget;
struct ITexture;

VK_CLASS()
class VKE_API vkPostProcessor : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:

public:
  vkPostProcessor();
  virtual ~vkPostProcessor();

  void SetInput(vkPostProcessOutput originOutput, ITexture *texture);
  const ITexture *GetInput(vkPostProcessOutput originOutput) const;
  ITexture *GetInput(vkPostProcessOutput originOutput);

  void SetFinalProcess(vkPostProcess *postProcess);
  bool BuildPostProcessing(IGraphics *graphics);

  void Render(IGraphics *graphics);

  IRenderTarget *GetOutput();


private:
  void BuildSet(vkPostProcess *process, std::set<vkPostProcess*> &processes);
  vkPostProcess *m_finalProcess;
  std::vector<vkPostProcess*> m_processes;
  ITexture *m_originInputs[ePPO_COUNT];
};

