

#include <cobalt/graphics/cspostprocess.hh>
#include <cobalt/graphics/cspostprocessor.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <set>
#include <map>
#include <string.h>


csPostProcess::csPostProcess()
  : csObject()
  , m_output(nullptr)
  , m_postProcessor(nullptr)
  , m_shader(nullptr)
{

}


csPostProcess::~csPostProcess()
{

}

void csPostProcess::SetShader(iShader *shader)
{
  CS_SET(m_shader, shader);
}


void csPostProcess::SetInputBindingName(int idx, const std::string &inputName)
{
  if (idx >= m_inputs.size())
  {
    return;
  }

  m_inputs[idx].m_inputName = inputName;
}


int csPostProcess::BindInput(csTextureWrapper *texture, const std::string &inputName)
{
  int res = (int)m_inputs.size();

  Input input;
  memset(&input, 0, sizeof(Input));
  input.m_inputName = inputName;
  input.m_inputSource = eIS_Texture;
  CS_SET(input.m_texture, texture);

  m_inputs.push_back(input);

  return res;
}

int csPostProcess::BindInput(csPostProcess *postProcess, int outputIdx, const std::string &inputName)
{
  int res = (int)m_inputs.size();

  Input input;
  memset(&input, 0, sizeof(Input));
  input.m_inputName = inputName;
  input.m_inputSource = eIS_PostProcess;
  CS_SET(input.m_postProcess, postProcess);
  input.m_postProcessOutput = outputIdx;

  m_inputs.push_back(input);

  return res;
}

int csPostProcess::BindInput(csPostProcessOutput originOutput, const std::string &inputName)
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

void csPostProcess::SetOutput(iRenderTarget *output)
{
  CS_SET(m_output, output);
}

iRenderTarget *csPostProcess::GetOutput()
{
  return m_output;
}

bool csPostProcess::BindShader(iGraphics *graphics)
{
  if (!m_shader)
  {
    return false;
  }
  graphics->SetShader(m_shader);
  return true;
}

bool csPostProcess::BindInputs(iGraphics *graphics)
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

    iTexture *inputTexture = input.m_texture->Get();
    if (input.m_attrInput)
    {
      csTextureUnit tu = graphics->BindTexture(inputTexture);
      input.m_attrInput->Set(tu);
    }
    switch (input.m_texture->Get()->GetType())
    {
    case eTT_Texture2D:
      {
        iTexture2D *txt2D = csQueryClass<iTexture2D>(inputTexture);
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

bool csPostProcess::BindOutput(iGraphics *graphics)
{
  if (!m_output)
  {
    return false;
  }
  graphics->SetRenderTarget(m_output);
  graphics->SetViewport(m_output);
  return true;
}

bool csPostProcess::Initialize(iGraphics *graphics)
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    Input &input = m_inputs[i];

    std::string inputName = input.m_inputName;
    std::string inputSizeName = inputName + std::string("Size");
    std::string inputSizeInvName = inputSizeName + std::string("Inv");
    input.m_attrInput = m_shader->GetAttribute(csShaderAttributeID(inputName));
    input.m_attrInputSize = m_shader->GetAttribute(csShaderAttributeID(inputSizeName));
    input.m_attrInputSizeInv = m_shader->GetAttribute(csShaderAttributeID(inputSizeInvName));
  }
  return true;
}

bool csPostProcess::Render(iGraphics *graphics)
{
  bool success = BindShader(graphics) && BindInputs(graphics) && BindOutput(graphics) ;
  if (!success)
  {
    return false;
  }
  graphics->BindValues();
  return true;
}

