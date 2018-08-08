
#include <cobalt/loaders/csmaterialassetcsfloader.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture.hh>
#include <map>



csMaterialAssetCSFLoader::csMaterialAssetCSFLoader()
  : csBaseCSFLoader()
{

}


csMaterialAssetCSFLoader::~csMaterialAssetCSFLoader()
{

}

bool csMaterialAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("material");
}

const csClass *csMaterialAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csMaterial::GetStaticClass();
}

iObject *csMaterialAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csMaterial *material = new csMaterial();

  const csfEntry *materialDefEntry = entry->GetEntry("materialDef");
  if (!materialDefEntry)
  {
    return material;
  }

  if (!HasLocator(materialDefEntry))
  {
    return material;
  }
  csMaterialDef *materialDef = csResourceManager::Get()->GetOrLoad<csMaterialDef>(GetLocator(materialDefEntry));
  if (!materialDef)
  {
    return material;
  }

  material->SetMaterialDef(materialDef);
  materialDef->Release();

  const csfEntry *parametersEntry = entry->GetEntry("parameters");
  if (parametersEntry)
  {
    for (const csfEntry *parameterEntry = parametersEntry->GetEntry("parameter");
         parameterEntry;
         parameterEntry = parameterEntry->GetSiblingEntry("parameter"))
    {
      csSize index;
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
          index = material->GetIndexByName(parametersEntry->GetAttribute("id"));
        }
        catch (const std::exception &e)
        {
          continue;
        }
      }



      csShaderParameterType type = materialDef->GetParamType(index);
      const csfEntry *valueEntry = parameterEntry->GetEntry();
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
        iTexture *texture = csResourceManager::Get()->GetOrLoad<iTexture>(GetLocator(valueEntry));
        material->Set(index, texture);
      }
    }
  }


  return material;
}


