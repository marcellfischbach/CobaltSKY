#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/vkpostprocessoutput.hh>
#include <set>
#include <valkyrie/graphics/vkpostprocessor.refl.hh>

class vkPostProcess;
struct iGraphics;
struct iRenderTarget;
struct iTexture;

VK_CLASS()
class VKE_API vkPostProcessor : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:

public:
  vkPostProcessor();
  virtual ~vkPostProcessor();

  void SetInput(vkPostProcessOutput originOutput, iTexture *texture);
  const iTexture *GetInput(vkPostProcessOutput originOutput) const;
  iTexture *GetInput(vkPostProcessOutput originOutput);

  void SetFinalProcess(vkPostProcess *postProcess);
  bool BuildPostProcessing(iGraphics *graphics);

  void Render(iGraphics *graphics);

  iRenderTarget *GetOutput();


private:
  void BuildSet(vkPostProcess *process, std::set<vkPostProcess*> &processes);
  vkPostProcess *m_finalProcess;
  std::vector<vkPostProcess*> m_processes;
  iTexture *m_originInputs[ePPO_COUNT];
};

