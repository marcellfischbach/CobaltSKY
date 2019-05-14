
#include <graphicsgl4/gl4programloader.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4enums.hh>
#include <graphicsgl4/gl4defines.hh>




csProgramGL4Loader::csProgramGL4Loader()
  : csBaseCSFLoader()
{
}

csProgramGL4Loader::~csProgramGL4Loader()
{

}


bool csProgramGL4Loader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  std::string tagName(entry->GetTagName());

  return tagName == std::string("program");
}



const cs::Class *csProgramGL4Loader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return csProgramGL4::GetStaticClass();
}

csResourceWrapper *csProgramGL4Loader::Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  if (std::string(entry->GetTagName()) != std::string("program"))
  {
    return nullptr;
  }
  CS_CHECK_GL_ERROR;

  const csfEntry *techniqueEntry = FindTechnique(entry);
  if (!techniqueEntry)
  {
    return nullptr;
  }

  csResourceManager *resourceManager = csResourceManager::Get();

  csProgramGL4 *program = new csProgramGL4();
  csProgramGL4Wrapper *programWrapper = new csProgramGL4Wrapper(program);
  for (const csfEntry *shaderEntry = techniqueEntry->GetEntry("shader");
       shaderEntry;
       shaderEntry = shaderEntry->GetSiblingEntry("shader"))
  {
    if (!shaderEntry->HasAttribute("locator"))
    {
      return nullptr;
    }
    csResourceLocator locator(shaderEntry->GetAttribute("locator"));
    csShaderGL4Wrapper *shader = resourceManager->GetOrLoad<csShaderGL4Wrapper>(locator);
    if (!shader || shader->IsNull())
    {
      program->Release();
      return nullptr;
    }
    program->AttachShader(shader);
  }

  if (!program->Link())
  {
    printf("Unable to link program: %s\n%s\n", locator.GetResourceFile().c_str(), program->GetLinkErrorLog().c_str());
    program->Release();
    return 0;
  }



  return programWrapper;
}

const csfEntry *csProgramGL4Loader::FindTechnique(const csfEntry *entry) const
{
  if (!entry)
  {
    return 0;
  }
  std::string entryName(entry->GetTagName());
  if (entryName == std::string("program"))
  {
    return FindTechnique(entry->GetEntry("techniques"));
  }
  else if (entryName == std::string("techniques"))
  {
    for (const csfEntry *techniqueEntry = entry->GetEntry("technique");
         techniqueEntry;
         techniqueEntry = techniqueEntry->GetSiblingEntry("technique"))
    {
      const csfEntry *technique = FindTechnique(techniqueEntry);
      if (technique)
      {
        return technique;
      }
    }
  }
  else if (entryName == std::string("technique"))
  {
    // do technique validation later.. for now just return the first technique 
    return entry;
  }
  return 0;
}
