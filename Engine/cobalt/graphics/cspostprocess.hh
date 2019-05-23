#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/cspostprocessoutput.hh>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cobalt/graphics/cspostprocess.refl.hh>




namespace cs
{
struct iRenderTarget;
struct iShaderAttribute;
class PostProcess;
class PostProcessor;
class TextureWrapper;


CS_CLASS()
class CSE_API PostProcess : public CS_SUPER(cs::Object)
{
  friend class cs::PostProcessor;
  CS_CLASS_GEN;
public:
  virtual ~PostProcess();

  virtual bool Render(cs::iGraphics * graphics);
  virtual bool Initialize(cs::iGraphics * graphics);

  int BindInput(cs::TextureWrapper * texture, const std::string & inputName = "");
  int BindInput(cs::PostProcess * postProcess, int outputIdx, const std::string & inputName = "");
  int BindInput(cs::ePostProcessOutput originOutput, const std::string & inputName = "");

  void SetOutput(cs::iRenderTarget * output);
  cs::iRenderTarget * GetOutput();

  cs::iShader * GetShader();
  const cs::iShader * GetShader() const;

protected:
  PostProcess();

  void SetInputBindingName(int idx, const std::string & name);
  void SetShader(cs::iShader * shader);

  bool BindShader(cs::iGraphics * graphics);
  bool BindInputs(cs::iGraphics * graphics);
  bool BindOutput(cs::iGraphics * graphics);

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
    std::string m_inputName;
    cs::TextureWrapper* m_texture;
    cs::PostProcess* m_postProcess;
    int m_postProcessOutput;
    cs::ePostProcessOutput m_originOutput;

    // shader attributes
    bool m_initialized;
    cs::iShaderAttribute* m_attrInput;
    cs::iShaderAttribute* m_attrInputSize;
    cs::iShaderAttribute* m_attrInputSizeInv;
  };

  std::vector<Input> m_inputs;

  cs::iRenderTarget* m_output;
  cs::PostProcessor* m_postProcessor;

  cs::iShader* m_shader;

};

}

CS_FORCEINLINE cs::iShader *cs::PostProcess::GetShader()
{
  return m_shader;
}

