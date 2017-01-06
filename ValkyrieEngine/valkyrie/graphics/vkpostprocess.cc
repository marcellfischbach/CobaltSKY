

#include <valkyrie/graphics/vkpostprocess.hh>
#include <valkyrie/graphics/vkpostprocessor.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <set>
#include <map>



vkPostProcess::vkPostProcess()
  : vkObject()
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

int vkPostProcess::BindInput(vkPostProcessOutput originOutput, const vkString &inputName)
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
    else if (input.m_inputSource == eIS_PostProcess)
    {
      input.m_texture = input.m_postProcess->GetOutput()->GetColorBuffer(input.m_postProcessOutput);
    }

    if (!input.m_texture)
    {
      continue;
    }

    if (input.m_attrInput)
    {
      vkTextureUnit tu = graphics->BindTexture(input.m_texture);
      input.m_attrInput->Set(tu);
    }
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
  bool success = BindShader(graphics) && BindInputs(graphics) && BindOutput(graphics) ;
  if (!success)
  {
    return false;
  }
  graphics->BindValues();
  return true;
}

