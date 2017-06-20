#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <set>
#include <cobalt/graphics/cspostprocessor.refl.hh>

class csPostProcess;
struct iGraphics;
struct iRenderTarget;
struct iTexture;

CS_CLASS()
class CSE_API csPostProcessor : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:

public:
  csPostProcessor();
  virtual ~csPostProcessor();

  void SetInput(csPostProcessOutput originOutput, iTexture *texture);
  const iTexture *GetInput(csPostProcessOutput originOutput) const;
  iTexture *GetInput(csPostProcessOutput originOutput);

  void SetFinalProcess(csPostProcess *postProcess);
  bool BuildPostProcessing(iGraphics *graphics);

  void Render(iGraphics *graphics);

  iRenderTarget *GetOutput();


private:
  void BuildSet(csPostProcess *process, std::set<csPostProcess*> &processes);
  csPostProcess *m_finalProcess;
  std::vector<csPostProcess*> m_processes;
  iTexture *m_originInputs[ePPO_COUNT];
};

