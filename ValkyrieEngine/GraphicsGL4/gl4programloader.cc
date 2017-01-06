
#include <GraphicsGL4/gl4programloader.hh>
#include <GraphicsGL4/gl4program.hh>
#include <GraphicsGL4/gl4shader.hh>
#include <GraphicsGL4/gl4enums.hh>
#include <GraphicsGL4/gl4defines.hh>




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

  return tagName == vkString("program");
}



const vkClass *vkProgramGL4Loader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkProgramGL4::GetStaticClass();
}

IObject *vkProgramGL4Loader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  if (vkString(element->Value()) != vkString("program"))
  {
    return 0;
  }
  VK_CHECK_GL_ERROR;

  TiXmlElement *techniqueElement = FindTechnique(element);
  if (!techniqueElement)
  {
    return 0;
  }

  vkResourceManager *resourceManager = vkResourceManager::Get();

  vkProgramGL4 *program = new vkProgramGL4();
  for (TiXmlElement *shaderElement = techniqueElement->FirstChildElement("shader");
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
