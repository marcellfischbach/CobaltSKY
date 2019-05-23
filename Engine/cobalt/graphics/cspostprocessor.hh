#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <set>
#include <cobalt/graphics/cspostprocessor.refl.hh>


namespace cs
{
struct iGraphics;
struct iRenderTarget;
class PostProcess;
class TextureWrapper;

CS_CLASS()
class CSE_API PostProcessor : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:

public:
  PostProcessor();
  virtual ~PostProcessor();

  void SetInput(cs::ePostProcessOutput originOutput, cs::TextureWrapper * texture);
  const cs::TextureWrapper* GetInput(cs::ePostProcessOutput originOutput) const;
  cs::TextureWrapper* GetInput(cs::ePostProcessOutput originOutput);

  void SetFinalProcess(cs::PostProcess * postProcess);
  bool BuildPostProcessing(cs::iGraphics * graphics);

  void Render(cs::iGraphics * graphics);

  cs::iRenderTarget* GetOutput();


private:
  void BuildSet(cs::PostProcess * process, std::set<cs::PostProcess*> & processes);
  cs::PostProcess* m_finalProcess;
  std::vector<cs::PostProcess*> m_processes;
  cs::TextureWrapper* m_originInputs[cs::ePPO_COUNT];
};

}
