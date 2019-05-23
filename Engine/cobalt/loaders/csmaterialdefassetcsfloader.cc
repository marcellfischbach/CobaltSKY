
#include <cobalt/loaders/csmaterialdefassetcsfloader.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/csengine.hh>



cs::MaterialDefAssetCSFLoader::MaterialDefAssetCSFLoader()
  : cs::BaseCSFLoader()
{
}

cs::MaterialDefAssetCSFLoader::~MaterialDefAssetCSFLoader()
{

}


bool cs::MaterialDefAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  std::string tagName = entry->GetTagName();

  return tagName == std::string("materialDef");
}



namespace
{
cs::eShaderParameterType get_shader_parameter_type(const std::string &name)
{
  if (name == std::string("float")) return cs::eSPT_Float;
  else if (name == std::string("vec2")) return cs::eSPT_Vector2;
  else if (name == std::string("vec3")) return cs::eSPT_Vector3;
  else if (name == std::string("vec4")) return cs::eSPT_Vector4;
  else if (name == std::string("int")) return cs::eSPT_Int;
  else if (name == std::string("ivec2")) return cs::eSPT_IVector2;
  else if (name == std::string("ivec3")) return cs::eSPT_IVector3;
  else if (name == std::string("ivec4")) return cs::eSPT_IVector4;
  else if (name == std::string("col")) return cs::eSPT_Color4;
  else if (name == std::string("mat3")) return cs::eSPT_Matrix3;
  else if (name == std::string("mat4")) return cs::eSPT_Matrix4;
  else if (name == std::string("texture")) return cs::eSPT_Texture;
  return cs::eSPT_Float;
}
}

const cs::Class *cs::MaterialDefAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::MaterialDefWrapper::GetStaticClass();
}


cs::ResourceWrapper *cs::MaterialDefAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  if (entry->GetTagName() != std::string("materialDef"))
  {
    return nullptr;
  }

  cs::MaterialDef *material = new cs::MaterialDef();

  const cs::file::Entry *passesEntry = entry->GetEntry("passes");
  if (!passesEntry)
  {
    material->Release();
    printf("Exit 1\n");
    return nullptr;
  }

  for (const cs::file::Entry *passEntry = passesEntry->GetEntry("pass");
       passEntry;
       passEntry = passEntry->GetSiblingEntry("pass"))
  {
    std::string passName;
    if (passEntry->HasAttribute("name"))
    {
      passName = passEntry->GetAttribute("name");
    }

    cs::eRenderPass renderPass;
    if (passName == std::string("GBuffer"))
    {
      renderPass = cs::eRP_GBuffer;
    }
    else if (passName == std::string("ShadowPSSM"))
    {
      renderPass = cs::eRP_ShadowPSSM;
    }
    else if (passName == std::string("ShadowCube"))
    {
      renderPass = cs::eRP_ShadowCube;
    }
    else if (passName == std::string("ForwardUnlit"))
    {
      renderPass = cs::eRP_ForwardUnlit;
    }
    else if (passName == std::string("Forward1Light"))
    {
      renderPass = cs::eRP_Forward1Light;
    }
    else if (passName == std::string("Forward2Light"))
    {
      renderPass = cs::eRP_Forward2Light;
    }
    else if (passName == std::string("Forward3Light"))
    {
      renderPass = cs::eRP_Forward3Light;
    }
    else if (passName == std::string("Forward4Light"))
    {
      renderPass = cs::eRP_Forward4Light;
    }
    else
    {
      material->Release();
      printf("Exit 2\n");
      return nullptr;
    }
    if (!HasLocator(passEntry))
    {
      material->Release();
      printf ("Exit 3\n");
      return nullptr;
    }
    cs::ShaderWrapper *shader = cs::ResourceManager::Get()->GetOrLoad<cs::ShaderWrapper>(GetLocator(passEntry));
    if (!shader)
    {
      material->Release();
      printf("Exit 4\n");
      return 0;
    }

    material->SetShader(renderPass, shader);
  }



  const cs::file::Entry *parametersEntry = entry->GetEntry("parameters");
  if (parametersEntry)
  {
    for (const cs::file::Entry *parameterEntry = parametersEntry->GetEntry("parameter");
         parameterEntry;
         parameterEntry = parameterEntry->GetSiblingEntry("parameter"))
    {
      std::string type, id, name;
      if (!parameterEntry->HasAttribute("type") || !parameterEntry->HasAttribute("name") || !parameterEntry->HasAttribute("id"))
      {
        continue;
      }
      type = parameterEntry->GetAttribute("type");
      id= parameterEntry->GetAttribute("id");
      name = parameterEntry->GetAttribute("name");

      cs::eShaderParameterType paramType;
      if (type == std::string("float"))
      {
        paramType = cs::eSPT_Float;
      }
      else if (type == std::string("vec2"))
      {
        paramType = cs::eSPT_Vector2;
      }
      else if (type == std::string("vec3"))
      {
        paramType = cs::eSPT_Vector3;
      }
      else if (type == std::string("vec4"))
      {
        paramType = cs::eSPT_Vector4;
      }
      else if (type == std::string("int"))
      {
        paramType = cs::eSPT_Int;
      }
      else if (type == std::string("ivec2"))
      {
        paramType = cs::eSPT_IVector2;
      }
      else if (type == std::string("ivec3"))
      {
        paramType = cs::eSPT_IVector3;
      }
      else if (type == std::string("ivec4"))
      {
        paramType = cs::eSPT_IVector4;
      }
      else if (type == std::string("col"))
      {
        paramType = cs::eSPT_Color4;
      }
      else if (type == std::string("mat3"))
      {
        paramType = cs::eSPT_Matrix3;
      }
      else if (type == std::string("mat4"))
      {
        paramType = cs::eSPT_Matrix4;
      }
      else if (type == std::string("texture"))
      {
        paramType = cs::eSPT_Texture;
      }
      else
      {
        continue;
      }

      csSize idx = material->RegisterParam(id, name, paramType);
      switch (paramType)
      {
      case cs::eSPT_Float:
        {
          float f = LoadFloat(parameterEntry);
          material->SetDefault(idx, f);
        }
        break;
      case cs::eSPT_Vector2:
        {
          cs::Vector2f v = LoadVector2f(parameterEntry);
          material->SetDefault(idx, v);
        }
        break;
      case cs::eSPT_Vector3:
        {
          cs::Vector3f v = LoadVector3f(parameterEntry);
          material->SetDefault(idx, v);
        }
        break;
      case cs::eSPT_Vector4:
        {
          cs::Vector4f v = LoadVector4f(parameterEntry);
          material->SetDefault(idx, v);
        }
        break;
      case cs::eSPT_Color4:
        {
          cs::Color4f v = LoadColor4f(parameterEntry);
          material->SetDefault(idx, v);
        }
        break;
      case cs::eSPT_Texture:
        {
          if (HasLocator(parameterEntry))
          {
            cs::eResourceLoadingMode rlm = GetResourceLoadingMode(parameterEntry);
            cs::TextureWrapper *texture = nullptr;
            switch (rlm)
            {
            case cs::eRLM_Shared:
              texture = cs::ResourceManager::Get()->GetOrLoad<cs::TextureWrapper>(GetLocator(parameterEntry));
              material->SetDefault(idx, texture);
              break;
            case cs::eRLM_Instance:
              texture = cs::ResourceManager::Get()->Load<cs::TextureWrapper>(GetLocator(parameterEntry));
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

  }



  return new cs::MaterialDefWrapper(material);
}






