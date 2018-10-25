
#include <cobalt/graphics/cspostprocessor.hh>
#include <cobalt/graphics/cspostprocess.hh>
#include <cobalt/graphics/cstexturewrapper.hh>
#include <string.h>

csPostProcessor::csPostProcessor()
  : csObject()
  , m_finalProcess(0)
{
  memset(m_originInputs, 0, sizeof(m_originInputs));
}

csPostProcessor::~csPostProcessor()
{

}


void csPostProcessor::SetInput(csPostProcessOutput originOutput, csTextureWrapper *texture)
{
  if (!texture)
  {
    return;
  }
  texture->AddRef();
  m_originInputs[originOutput] = texture;
}

const csTextureWrapper *csPostProcessor::GetInput(csPostProcessOutput originOutput) const
{
  return m_originInputs[originOutput];
}

csTextureWrapper *csPostProcessor::GetInput(csPostProcessOutput originOutput)
{
  return m_originInputs[originOutput];
}


void csPostProcessor::SetFinalProcess(csPostProcess *postProcess)
{
  CS_SET(m_finalProcess, postProcess);
}

void csPostProcessor::BuildSet(csPostProcess *process, std::set<csPostProcess*> &processes)
{
  processes.insert(process);
  for (size_t j = 0, jn = process->m_inputs.size(); j < jn; ++j)
  {
    csPostProcess::Input &input = process->m_inputs[j];
    if (input.m_inputSource == csPostProcess::eIS_PostProcess)
    {
      BuildSet(input.m_postProcess, processes);
    }
  }
}


bool csPostProcessor::BuildPostProcessing(iGraphics *graphics)
{
  // build a set containing all post processes that are used within the process tree
  std::set<csPostProcess*> processes;
  BuildSet(m_finalProcess, processes);

  // build a map containing all needed direct references to other post processes per post process
  // map<pp -> set<pp>>
  std::map<csPostProcess*, std::set<csPostProcess*>> references;
  for (csPostProcess *process : processes)
  {
    for (size_t j = 0, jn = process->m_inputs.size(); j < jn; ++j)
    {
      std::set<csPostProcess*> &refs = references[process];

      csPostProcess::Input &input = process->m_inputs[j];
      if (input.m_inputSource == csPostProcess::eIS_PostProcess)
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
    std::set<csPostProcess*> toBeRemoved;
    for (std::map<csPostProcess*, std::set<csPostProcess*>>::iterator it = references.begin();
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
    for (std::set<csPostProcess*>::iterator it = toBeRemoved.begin(); it != toBeRemoved.end(); ++it)
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
    for (std::map<csPostProcess*, std::set<csPostProcess*>>::iterator it = references.begin();
         it != references.end();
         ++it)
    {
      for (std::set<csPostProcess*>::iterator it2 = toBeRemoved.begin(); it2 != toBeRemoved.end(); ++it2)
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


iRenderTarget *csPostProcessor::GetOutput()
{
  if (m_finalProcess)
  {
    return m_finalProcess->GetOutput();
  }
  return 0;
}

void csPostProcessor::Render(iGraphics *graphics)
{
  for (size_t i = 0, in = m_processes.size(); i < in; ++i)
  {
    if (!m_processes[i]->Render(graphics))
    {
      return;
    }
  }
}


