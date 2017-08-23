
#include <cobalt/loaders/csmaterialdefassetxmlloader.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/csengine.hh>



csMaterialDefAssetXMLLoader::csMaterialDefAssetXMLLoader()
  : csBaseXMLLoader()
{
}

csMaterialDefAssetXMLLoader::~csMaterialDefAssetXMLLoader()
{

}


bool csMaterialDefAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName(element->Value());

  return tagName == std::string("materialDef");
}



namespace
{
csShaderParameterType get_shader_parameter_type(const std::string &name)
{
  if (name == std::string("float")) return eSPT_Float;
  else if (name == std::string("vec2")) return eSPT_Vector2;
  else if (name == std::string("vec3")) return eSPT_Vector3;
  else if (name == std::string("vec4")) return eSPT_Vector4;
  else if (name == std::string("int")) return eSPT_Int;
  else if (name == std::string("ivec2")) return eSPT_IVector2;
  else if (name == std::string("ivec3")) return eSPT_IVector3;
  else if (name == std::string("ivec4")) return eSPT_IVector4;
  else if (name == std::string("col")) return eSPT_Color4;
  else if (name == std::string("mat3")) return eSPT_Matrix3;
  else if (name == std::string("mat4")) return eSPT_Matrix4;
  else if (name == std::string("texture")) return eSPT_Texture;
  return eSPT_Float;
}
}

const csClass *csMaterialDefAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csMaterialDef::GetStaticClass();
}


iObject *csMaterialDefAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  if (std::string(element->Value()) != std::string("materialDef"))
  {
    return 0;
  }
  
  csMaterialDef *material = new csMaterialDef();

  TiXmlElement *passesElement = element->FirstChildElement("passes");
  if (!passesElement)
  {
    material->Release();
    printf("Exit 1\n");
    return 0;
  }

  for (TiXmlElement *passElement = passesElement->FirstChildElement("pass");
       passElement;
       passElement = passElement->NextSiblingElement("pass"))
  {
    std::string passName;
    if (passElement->Attribute("name"))
    {
      passName = std::string(passElement->Attribute("name"));
    }

    csRenderPass renderPass;
    if (passName == std::string("GBuffer"))
    {
      renderPass = eRP_GBuffer;
    }
    else if (passName == std::string("ShadowPSSM"))
    {
      renderPass = eRP_ShadowPSSM;
    }
    else if (passName == std::string("ShadowCube"))
    {
      renderPass = eRP_ShadowCube;
    }
    else if (passName == std::string("ForwardUnlit"))
    {
      renderPass = eRP_ForwardUnlit;
    }
    else if (passName == std::string("Forward1Light"))
    {
      renderPass = eRP_Forward1Light;
    }
    else if (passName == std::string("Forward2Light"))
    {
      renderPass = eRP_Forward2Light;
    }
    else if (passName == std::string("Forward3Light"))
    {
      renderPass = eRP_Forward3Light;
    }
    else if (passName == std::string("Forward4Light"))
    {
      renderPass = eRP_Forward4Light;
    }
    else
    {
      material->Release();
      printf("Exit 2\n");
      return 0;
    }

    csResourceLocator shaderLocator(passElement->GetText());
    iShader *shader = csResourceManager::Get()->GetOrLoad<iShader>(shaderLocator);
    if (!shader)
    {
      material->Release();
      printf("Exit 3\n");
      return 0;
    }

    material->SetShader(renderPass, shader);
  }



  TiXmlElement *parametersElement = element->FirstChildElement("parameters");
  if (parametersElement)
  {
    for (TiXmlElement *parameterElement = parametersElement->FirstChildElement("parameter");
         parameterElement;
         parameterElement = parameterElement->NextSiblingElement("parameter"))
    {
      std::string type, name;
      if (!parameterElement->Attribute("type") || !parameterElement->Attribute("name"))
      {
        continue;
      }
      type = std::string(parameterElement->Attribute("type"));
      name = std::string(parameterElement->Attribute("name"));
      std::string defaultValue = std::string(parameterElement->GetText());

      csShaderParameterType paramType;
      if (type == std::string("float"))
      {
        paramType = eSPT_Float;
      }
      else if (type == std::string("vec2"))
      {
        paramType = eSPT_Vector2;
      }
      else if (type == std::string("vec3"))
      {
        paramType = eSPT_Vector3;
      }
      else if (type == std::string("vec4"))
      {
        paramType = eSPT_Vector4;
      }
      else if (type == std::string("int"))
      {
        paramType = eSPT_Int;
      }
      else if (type == std::string("ivec2"))
      {
        paramType = eSPT_IVector2;
      }
      else if (type == std::string("ivec3"))
      {
        paramType = eSPT_IVector3;
      }
      else if (type == std::string("ivec4"))
      {
        paramType = eSPT_IVector4;
      }
      else if (type == std::string("col"))
      {
        paramType = eSPT_Color4;
      }
      else if (type == std::string("mat3"))
      {
        paramType = eSPT_Matrix3;
      }
      else if (type == std::string("mat4"))
      {
        paramType = eSPT_Matrix4;
      }
      else if (type == std::string("texture"))
      {
        paramType = eSPT_Texture;
      }
      else
      {
        continue;
      }

      csSize idx = material->RegisterParam(name, paramType);
      switch (paramType)
      {
      case eSPT_Float:
        {
          float f = LoadFloat(parameterElement->GetText());
          material->SetDefault(idx, f);
        }
        break;
      case eSPT_Vector2:
        {
          csVector2f v = LoadVector2f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Vector3:
        {
          csVector3f v = LoadVector3f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Vector4:
        {
          csVector4f v = LoadVector4f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Color4:
        {
          csColor4f v = LoadColor4f(parameterElement->GetText());
          material->SetDefault(idx, v);
        }
        break;
      case eSPT_Texture:
        {
          csResourceLoadingMode rlm = GetResourceLoadingMode(parameterElement);
          iTexture *texture = 0;
          switch (rlm)
          {
          case eRLM_Shared:
            texture = csResourceManager::Get()->GetOrLoad<iTexture>(csResourceLocator(parameterElement->GetText()));
            material->SetDefault(idx, texture);
            break;
          case eRLM_Instance:
            texture = csResourceManager::Get()->Load<iTexture>(csResourceLocator(parameterElement->GetText()));
            material->SetDefault(idx, texture);
            if (texture) texture->Release();
            break;
          default:
            printf("not implemented yet.");

          }
        }
      }
    }

  }



  return material;
}






