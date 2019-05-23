
#include <cobalt/loaders/csmaterialassetcsfloader.hh>
#include <csrefl/classregistry.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture.hh>
#include <map>



cs::MaterialAssetCSFLoader::MaterialAssetCSFLoader()
  : cs::BaseCSFLoader()
{

}


cs::MaterialAssetCSFLoader::~MaterialAssetCSFLoader()
{

}

bool cs::MaterialAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return entry->GetTagName() == std::string("material");
}

const cs::Class *cs::MaterialAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return cs::MaterialWrapper::GetStaticClass();
}

cs::ResourceWrapper *cs::MaterialAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::Material *material = new cs::Material();

  const cs::file::Entry *materialDefEntry = entry->GetEntry("materialDef");
  if (!materialDefEntry)
  {
    return new cs::MaterialWrapper(material);
  }

  if (!HasLocator(materialDefEntry))
  {
    return new cs::MaterialWrapper(material);
  }
  cs::MaterialDefWrapper *materialDefWrapper = cs::ResourceManager::Get()->GetOrLoad<cs::MaterialDefWrapper>(GetLocator(materialDefEntry));
  if (!materialDefWrapper || !materialDefWrapper->IsValid())
  {
    return new cs::MaterialWrapper(material);
  }
  cs::MaterialDef *materialDef = materialDefWrapper->Get();

  material->SetMaterialDef(materialDefWrapper);
  materialDefWrapper->Release();

  const cs::file::Entry *parametersEntry = entry->GetEntry("parameters");
  if (parametersEntry)
  {
    for (const cs::file::Entry *parameterEntry = parametersEntry->GetEntry("parameter");
         parameterEntry;
         parameterEntry = parameterEntry->GetSiblingEntry("parameter"))
    {
      csSize index = ~0x00;
      if (parameterEntry->HasAttribute("id") )
      {
        try
        {
          index = material->GetIndex(parameterEntry->GetAttribute("id"));
        }
        catch (const std::exception &e)
        {
          continue;
        }
      }
      else if (parametersEntry->HasAttribute("name"))
      {
        try
        {
          index = material->GetIndexByName(parametersEntry->GetAttribute("name"));
        }
        catch (const std::exception &e)
        {
          continue;
        }
      }
      if (index == ~0x00)
      {
        continue;
      }



      cs::eShaderParameterType type = materialDef->GetParamType(index);
      const cs::file::Entry *valueEntry = parameterEntry->GetEntry();
      if (!valueEntry)
      {
        continue;
      }

      std::string tagName = valueEntry->GetTagName();
      if (tagName == std::string("float"))
      {
        material->Set(index, LoadFloat(valueEntry));
      }
      else if (tagName == std::string("float2"))
      {
        material->Set(index, LoadVector2f(valueEntry));
      }
      else if (tagName == std::string("float3"))
      {
        material->Set(index, LoadVector3f(valueEntry));
      }
      else if (tagName == std::string("float4"))
      {
        material->Set(index, LoadVector4f(valueEntry));
      }
      else if (tagName == std::string("color4"))
      {
        material->Set(index, LoadColor4f(valueEntry));
      }
      else if (tagName == std::string("locator") && HasLocator(valueEntry))
      {
        cs::TextureWrapper *texture = cs::ResourceManager::Get()->GetOrLoad<cs::TextureWrapper>(GetLocator(valueEntry));
        material->Set(index, texture);
      }
    }
  }


  return new cs::MaterialWrapper(material);
}


