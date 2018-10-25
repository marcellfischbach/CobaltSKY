#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <set>
#include <cobalt/graphics/cspostprocessor.refl.hh>

class csPostProcess;
struct iGraphics;
struct iRenderTarget;
struct csTextureWrapper;

CS_CLASS()
class CSE_API csPostProcessor : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:

public:
  csPostProcessor();
  virtual ~csPostProcessor();

  void SetInput(csPostProcessOutput originOutput, csTextureWrapper *texture);
  const csTextureWrapper *GetInput(csPostProcessOutput originOutput) const;
  csTextureWrapper *GetInput(csPostProcessOutput originOutput);

  void SetFinalProcess(csPostProcess *postProcess);
  bool BuildPostProcessing(iGraphics *graphics);

  void Render(iGraphics *graphics);

  iRenderTarget *GetOutput();


private:
  void BuildSet(csPostProcess *process, std::set<csPostProcess*> &processes);
  csPostProcess *m_finalProcess;
  std::vector<csPostProcess*> m_processes;
  csTextureWrapper *m_originInputs[ePPO_COUNT];
};

