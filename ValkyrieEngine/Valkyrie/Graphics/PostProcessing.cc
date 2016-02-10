

#include <Valkyrie/Graphics/PostProcessing.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <set>
#include <map>


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

void vkPostProcessor::BuildSet(vkPostProcess *process, std::set<vkPostProcess*> &processes)
{
  processes.insert(process);
  for (size_t j = 0, jn = process->m_inputs.size(); j < jn; ++j)
  {
    vkPostProcess::Input &input = process->m_inputs[j];
    if (input.m_inputSource == vkPostProcess::eIS_PostProcess)
    {
      BuildSet(input.m_postProcess, processes);
    }
  }
}


bool vkPostProcessor::BuildPostProcessing(IGraphics *graphics)
{
  // build a set containing all post processes that are used within the process tree
  std::set<vkPostProcess*> processes;
  BuildSet(m_finalProcess, processes);

  // build a map containing all needed direct references to other post processes per post process
  // map<pp -> set<pp>>
  std::map<vkPostProcess*, std::set<vkPostProcess*>> references;
  for (vkPostProcess *process : processes)
  {
    for (size_t j = 0, jn = process->m_inputs.size(); j < jn; ++j)
    {
      std::set<vkPostProcess*> &refs = references[process];

      vkPostProcess::Input &input = process->m_inputs[j];
      if (input.m_inputSource == vkPostProcess::eIS_PostProcess)
      {
        refs.insert(input.m_postProcess);
      }
    }
  }

  // now building the ordered list
  m_processes.clear();
  while (!references.empty())
  {
    // the first find all processes that have no reference to other processes
    // theses processes are put into the list (they can be rendered without further need)
    std::set<vkPostProcess*> toBeRemoved;
    for (std::map<vkPostProcess*, std::set<vkPostProcess*>>::iterator it = references.begin();
    it != references.end();
      ++it)
    {
      if (it->second.size() == 0)
      {
        m_processes.push_back(it->first);
        toBeRemoved.insert(it->first);
      }
    }

    // those PPs are not needed in any further iteration so remove them
    for (std::set<vkPostProcess*>::iterator it = toBeRemoved.begin(); it != toBeRemoved.end(); ++it)
    {
      references.erase(*it);
    }
    if (references.empty())
    {
      break;
    }

    // now remove those PPs from the reference list ... 
    // this should make at least one other PP free (that is it has no reference to another PP)

    // a fallback hook for not crashing when loops are built
    bool onePPFreed = false;
    for (std::map<vkPostProcess*, std::set<vkPostProcess*>>::iterator it = references.begin();
    it != references.end();
      ++it)
    {
      for (std::set<vkPostProcess*>::iterator it2 = toBeRemoved.begin(); it2 != toBeRemoved.end(); ++it2)
      {
        it->second.erase(*it2);
        if (it->second.empty())
        {
          onePPFreed = true;
        }
      }
    }
    if (!onePPFreed)
    {
      // in this iteration no other PP has been freed. there must be a loop -> error
      return false;
    }
  }



  // assign the post processor
  for (size_t i = 0, in = m_processes.size(); i < in; ++i)
  {
    m_processes[i]->m_postProcessor = this;
  }

  for (size_t i = 0, in = m_processes.size(); i < in; ++i)
  {
    m_processes[i]->Initialize(graphics);
  }
  
  return true;
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
  return BindShader(graphics) && BindInputs(graphics) && BindOutput(graphics);
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



