

#include <Valkyrie/Graphics/PostProcessing.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>


vkPostProcessor::vkPostProcessor()
  : vkObject()
  , m_finalProcess(0)
{
  memset(m_originInputs, 0, sizeof(m_originInputs));
}

vkPostProcessor::~vkPostProcessor()
{

}


void vkPostProcessor::SetInput(OriginOutput originOutput, ITexture *texture)
{
  if (!texture)
  {
    return;
  }
  texture->AddRef();
  m_originInputs[originOutput] = texture;
}

const ITexture *vkPostProcessor::GetInput(OriginOutput originOutput) const
{
  return m_originInputs[originOutput];
}

ITexture *vkPostProcessor::GetInput(OriginOutput originOutput)
{
  return m_originInputs[originOutput];
}


void vkPostProcessor::SetFinalProcess(vkPostProcess *postProcess)
{
  VK_SET(m_finalProcess, postProcess);
}


void vkPostProcessor::BuildPostProcessing(IGraphics *graphics)
{
  m_processes.clear();

  m_processes.push_back(m_finalProcess);

  // assign the post processor
  for (size_t i = 0, in = m_processes.size(); i < in; ++i)
  {
    m_processes[i]->m_postProcessor = this;
  }

  for (size_t i = 0, in = m_processes.size(); i < in; ++i)
  {
    m_processes[i]->Initialize(graphics);
  }
}


IRenderTarget *vkPostProcessor::GetOutput()
{
  if (m_finalProcess)
  {
    return m_finalProcess->GetOutput();
  }
  return 0;
}

void vkPostProcessor::Render(IGraphics *graphics)
{
  for (size_t i = 0, in = m_processes.size(); i < in; ++i)
  {
    if (!m_processes[i]->Render(graphics))
    {
      return;
    }
  }
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

int vkPostProcess::BindInput(vkPostProcessor::OriginOutput originOutput, const vkString &inputName)
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

IRenderTarget *vkPostProcess::GetOutput()
{
  return m_output;
}

bool vkPostProcess::BindShader(IGraphics *graphics)
{
  if (!m_shader)
  {
    return false;
  }
  graphics->SetShader(m_shader);
  return true;
}

bool vkPostProcess::BindInputs(IGraphics *graphics)
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    Input &input = m_inputs[i];
    if (input.m_inputSource == eIS_Origin)
    {
      input.m_texture = m_postProcessor->GetInput(input.m_originOutput);
    }

    if (!input.m_texture || !input.m_attrInput)
    {
      continue;
    }

    vkTextureUnit tu = graphics->BindTexture(input.m_texture);
    input.m_attrInput->Set(tu);
    switch (input.m_texture->GetType())
    {
    case eTT_Texture2D:
      {
        ITexture2D *txt2D = vkQueryClass<ITexture2D>(input.m_texture);
        if (input.m_attrInputSize)
        {
          input.m_attrInputSize->Set((float)txt2D->GetWidth(), (float)txt2D->GetHeight());
        }
        if (input.m_attrInputSizeInv)
        {
          input.m_attrInputSizeInv->Set(1.0f / (float)txt2D->GetWidth(), 1.0f / (float)txt2D->GetHeight());
        }
      }
      break;
    }
  }
  return true;
}

bool vkPostProcess::BindOutput(IGraphics *graphics)
{
  if (!m_output)
  {
    return false;
  }
  graphics->SetRenderTarget(m_output);
  graphics->SetViewport(m_output);
  return true;
}

bool vkPostProcess::Initialize(IGraphics *graphics)
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    Input &input = m_inputs[i];

    vkString inputName = input.m_inputName;
    vkString inputSizeName = inputName + vkString("Size");
    vkString inputSizeInvName = inputSizeName + vkString("Inv");

    input.m_attrInput = m_shader->GetAttribute(vkShaderAttributeID(inputName));
    input.m_attrInputSize = m_shader->GetAttribute(vkShaderAttributeID(inputSizeName));
    input.m_attrInputSizeInv = m_shader->GetAttribute(vkShaderAttributeID(inputSizeInvName));
  }
  return true;
}

bool vkPostProcess::Render(IGraphics *graphics)
{
  return BindShader(graphics) && BindInputs(graphics) && BindOutput (graphics);
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


bool vkGenericShaderPostProcess::Render(IGraphics *graphics)
{
  if (!vkPostProcess::Render(graphics))
  {
    return false;
  }

  graphics->RenderFullScreenFrame();
  return true;
}



