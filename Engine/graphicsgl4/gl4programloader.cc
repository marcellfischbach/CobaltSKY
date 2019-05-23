
#include <graphicsgl4/gl4programloader.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4enums.hh>
#include <graphicsgl4/gl4defines.hh>




cs::ProgramGL4Loader::ProgramGL4Loader()
  : cs::BaseCSFLoader()
{
}

cs::ProgramGL4Loader::~ProgramGL4Loader()
{

}


bool cs::ProgramGL4Loader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  std::string tagName(entry->GetTagName());

  return tagName == std::string("program");
}



const cs::Class *cs::ProgramGL4Loader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::ProgramGL4::GetStaticClass();
}

cs::ResourceWrapper *cs::ProgramGL4Loader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (std::string(entry->GetTagName()) != std::string("program"))
  {
    return nullptr;
  }
  CS_CHECK_GL_ERROR;

  const cs::file::Entry *techniqueEntry = FindTechnique(entry);
  if (!techniqueEntry)
  {
    return nullptr;
  }

  cs::ResourceManager *resourceManager = cs::ResourceManager::Get();

  cs::ProgramGL4 *program = new cs::ProgramGL4();
  cs::ProgramGL4Wrapper *programWrapper = new cs::ProgramGL4Wrapper(program);
  for (const cs::file::Entry *shaderEntry = techniqueEntry->GetEntry("shader");
       shaderEntry;
       shaderEntry = shaderEntry->GetSiblingEntry("shader"))
  {
    if (!shaderEntry->HasAttribute("locator"))
    {
      return nullptr;
    }
    cs::ResourceLocator locator(shaderEntry->GetAttribute("locator"));
    cs::ShaderGL4Wrapper *shader = resourceManager->GetOrLoad<cs::ShaderGL4Wrapper>(locator);
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

const cs::file::Entry *cs::ProgramGL4Loader::FindTechnique(const cs::file::Entry *entry) const
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
    for (const cs::file::Entry *techniqueEntry = entry->GetEntry("technique");
         techniqueEntry;
         techniqueEntry = techniqueEntry->GetSiblingEntry("technique"))
    {
      const cs::file::Entry *technique = FindTechnique(techniqueEntry);
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
