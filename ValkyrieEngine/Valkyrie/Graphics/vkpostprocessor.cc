
#include <Valkyrie/Graphics/vkpostprocessor.hh>


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


