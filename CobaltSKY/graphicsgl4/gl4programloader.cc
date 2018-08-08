
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


bool csProgramGL4Loader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName(entry->GetTagName());

  return tagName == std::string("program");
}



const csClass *csProgramGL4Loader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csProgramGL4::GetStaticClass();
}

iObject *csProgramGL4Loader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  if (std::string(entry->GetTagName()) != std::string("program"))
  {
    return 0;
  }
  CS_CHECK_GL_ERROR;

  const csfEntry *techniqueEntry = FindTechnique(entry);
  if (!techniqueEntry)
  {
    return 0;
  }

  csResourceManager *resourceManager = csResourceManager::Get();

  csProgramGL4 *program = new csProgramGL4();
  for (const csfEntry *shaderEntry = techniqueEntry->GetEntry("shader");
       shaderEntry;
       shaderEntry = shaderEntry->GetSiblingEntry("shader"))
  {
    if (!shaderEntry->HasAttribute("locator"))
    {
      return 0;
    }
    csResourceLocator locator(shaderEntry->GetAttribute("locator"));
    csShaderGL4 *shader = resourceManager->GetOrLoad<csShaderGL4>(locator);
    if (!shader)
    {
      program->Release();
      return 0;
    }
    program->AttachShader(shader);
  }

  if (!program->Link())
  {
    printf("Unable to link program: %s\n%s\n", locator.GetResourceFile().c_str(), program->GetLinkErrorLog().c_str());
    program->Release();
    return 0;
  }



  return program;
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
