

#include <Valkyrie/Graphics/PostProcessing.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>


vkPostProcessor::vkPostProcessor()
  : vkObject()
{

}

vkPostProcessor::~vkPostProcessor()
{

}


void vkPostProcessor::SetInput(const vkString &inputName, ITexture *texture)
{
  if (!texture)
  {
    return;
  }
  texture->AddRef();
  m_inputs[inputName] = texture;
}

const ITexture *vkPostProcessor::GetInput(const vkString &name) const
{
  std::map<vkString, ITexture*>::const_iterator it = m_inputs.find(name);
  if (it == m_inputs.end())
  {
    return 0;
  }

  return it->second;
}

ITexture *vkPostProcessor::GetInput(const vkString &name) 
{
  std::map<vkString, ITexture*>::iterator it = m_inputs.find(name);
  if (it == m_inputs.end())
  {
    return 0;
  }

  return it->second;
}



void vkPostProcessor::SetFinalProcess(vkPostProcess *postProcess)
{
  VK_SET(m_finalProcess, postProcess);
}


void vkPostProcessor::BuildPostProcessing()
{
  // TODO: implement me
}






vkPostProcess::vkPostProcess()
  : vkObject ()
  , m_output(0)
  , m_postProcessor(0)
  , m_shader(0)
{

}


vkPostProcess::~vkPostProcess()
{

}

void vkPostProcess::SetShader(IShader *shader)
{
  VK_SET(m_shader, shader);
}


void vkPostProcess::SetInputBindingName(int idx, const vkString &inputName)
{
  if (idx >= m_inputs.size())
  {
    return;
  }

  m_inputs[idx].m_inputName = inputName;
}


int vkPostProcess::BindInput(ITexture *texture, const vkString &inputName)
{
  int res = (int)m_inputs.size();

  Input input;
  memset(&input, 0, sizeof(Input));
  input.m_inputName = inputName;
  input.m_inputSource = eIS_Texture;
  VK_SET(input.m_texture, texture);

  m_inputs.push_back(input);

  return res;
}

int vkPostProcess::BindInput(vkPostProcess *postProcess, int outputIdx, const vkString &inputName)
{
  int res = (int)m_inputs.size();

  Input input;
  memset(&input, 0, sizeof(Input));
  input.m_inputName = inputName;
  input.m_inputSource = eIS_PostProcess;
  VK_SET(input.m_postProcess, postProcess);
  input.m_postProcessOutput = outputIdx;

  m_inputs.push_back(input);

  return res;
}

int vkPostProcess::BindInput(const vkString &originOutput, const vkString &inputName)
{
  int res = (int)m_inputs.size();

  Input input;
  memset(&input, 0, sizeof(Input));
  input.m_inputName = inputName;
  input.m_inputSource = eIS_Origin;
  input.m_originOutput = originOutput;

  m_inputs.push_back(input);

  return res;
}

void vkPostProcess::SetOutput(IRenderTarget *output)
{
  VK_SET(m_output, output);
}








vkGenericShaderPostProcess::vkGenericShaderPostProcess()
  : vkPostProcess()
{

}

vkGenericShaderPostProcess::~vkGenericShaderPostProcess()
{

}


void vkGenericShaderPostProcess::SetShader(IShader *shader)
{
  vkPostProcess::SetShader(shader);
}


void vkGenericShaderPostProcess::Render(IGraphics *graphics)
{

}



