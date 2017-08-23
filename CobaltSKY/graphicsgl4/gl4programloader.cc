
#include <graphicsgl4/gl4programloader.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4enums.hh>
#include <graphicsgl4/gl4defines.hh>




csProgramGL4Loader::csProgramGL4Loader()
  : csBaseXMLLoader()
{
}

csProgramGL4Loader::~csProgramGL4Loader()
{

}


bool csProgramGL4Loader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName(element->Value());

  return tagName == std::string("program");
}



const csClass *csProgramGL4Loader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csProgramGL4::GetStaticClass();
}

iObject *csProgramGL4Loader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (std::string(element->Value()) != std::string("program"))
  {
    return 0;
  }
  CS_CHECK_GL_ERROR;

  TiXmlElement *techniqueElement = FindTechnique(element);
  if (!techniqueElement)
  {
    return 0;
  }

  csResourceManager *resourceManager = csResourceManager::Get();

  csProgramGL4 *program = new csProgramGL4();
  for (TiXmlElement *shaderElement = techniqueElement->FirstChildElement("shader");
       shaderElement;
       shaderElement = shaderElement->NextSiblingElement("shader"))
  {
    csResourceLocator locator(std::string(shaderElement->GetText()));
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

TiXmlElement *csProgramGL4Loader::FindTechnique(TiXmlElement *element) const
{
  if (!element)
  {
    return 0;
  }
  std::string elementName(element->Value());
  if (elementName == std::string("program"))
  {
    return FindTechnique(element->FirstChildElement("techniques"));
  }
  else if (elementName == std::string("techniques"))
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
  else if (elementName == std::string("technique"))
  {
    // do technique validation later.. for now just return the first technique 
    return element;
  }
  return 0;
}
