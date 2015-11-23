
#include <GraphicsGL4/ShaderLoader.hh>
#include <GraphicsGL4/Shader.hh>
#include <GraphicsGL4/Enums.hh>
#include <GraphicsGL4/DefinesGL4.hh>


vkShaderGL4Loader::vkShaderGL4Loader()
{
  VK_CLASS_GEN_CONSTR;
}

vkShaderGL4Loader::~vkShaderGL4Loader()
{
}

bool vkShaderGL4Loader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *) const
{
  vkString ext = file->GetExtension();
  return
    ext == vkString("vert") ||
    ext == vkString("ctrl") ||
    ext == vkString("eval") ||
    ext == vkString("geom") ||
    ext == vkString("frag") ||
    ext == vkString("comp");
}

IObject *vkShaderGL4Loader::Load(IFile *file, const vkResourceLocator &locator, IObject *) const
{
  vkString ext = file->GetExtension();

  vkShaderType type;
  if (ext == vkString("vert"))
  {
    type = eST_Vertex;
  }
  else if (ext == vkString("ctrl"))
  {
    type = eST_TessCtrl;
  }
  else if (ext == vkString("eval"))
  {
    type = eST_TessEval;
  }
  else if (ext == vkString("geom"))
  {
    type = eST_Geometry;
  }
  else if (ext == vkString("frag"))
  {
    type = eST_Fragment;
  }
  else if (ext == vkString("comp"))
  {
    type = eST_Compute;
  }
  else
  {
    return 0;
  }



  vkSize length = file->GetLength();

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';


  vkString source(buffer);
  delete[] buffer;

  vkShaderGL4 *shader = new vkShaderGL4();
  shader->SetSource(source);
  shader->SetShaderType(type);

  if (!shader->Compile())
  {
    vkString log = shader->GetCompileErrorLog();
    printf("Code:\n%s\n", source.c_str());
    printf("Log:\n%s\n", log.c_str());
    shader->Release();
    return 0;
  }

  return shader;
}



vkProgramGL4Loader::vkProgramGL4Loader()
  : vkBaseXMLLoader()
{
}

vkProgramGL4Loader::~vkProgramGL4Loader()
{

}


bool vkProgramGL4Loader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());

  return tagName == vkString("program") || tagName == vkString("programs");
}




IObject *vkProgramGL4Loader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  VK_CHECK_GL_ERROR;
  TiXmlElement *programElement = FindElement(element, "program", locator.GetResourceName());
  if (!programElement)
  {
    return 0;
  }

  TiXmlElement *technique = FindTechnique(programElement);
  if (!technique)
  {
    return 0;
  }

  vkResourceManager *resourceManager = vkResourceManager::Get();

  vkProgramGL4 *program = new vkProgramGL4();
  for (TiXmlElement *shaderElement = technique->FirstChildElement("shader");
  shaderElement;
    shaderElement = shaderElement->NextSiblingElement("shader"))
  {
    vkResourceLocator locator(vkString(shaderElement->GetText()));
    vkShaderGL4 *shader = resourceManager->GetOrLoad<vkShaderGL4>(locator);
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

TiXmlElement *vkProgramGL4Loader::FindTechnique(TiXmlElement *element) const
{
  if (!element)
  {
    return 0;
  }
  vkString elementName(element->Value());
  if (elementName == vkString("program"))
  {
    return FindTechnique(element->FirstChildElement("techniques"));
  }
  else if (elementName == vkString("techniques"))
  {
    for (TiXmlElement *techniqueElement = element->FirstChildElement("technique");
    techniqueElement;
      techniqueElement = techniqueElement->NextSiblingElement("technique"))
    {
      TiXmlElement *technique = FindTechnique(techniqueElement);
      if (technique)
      {
        return technique;
      }
    }
  }
  else if (elementName == vkString("technique"))
  {
    // do technique validation later.. for now just return the first technique 
    return element;
  }
  return 0;
}
