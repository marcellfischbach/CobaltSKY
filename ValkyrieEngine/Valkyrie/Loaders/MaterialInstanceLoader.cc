
#include <Valkyrie/Loaders/MaterialInstanceLoader.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <map>

vkMaterialInstanceAssetLoader::vkMaterialInstanceAssetLoader()
  : IAssetLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkMaterialInstanceAssetLoader::~vkMaterialInstanceAssetLoader()
{

}

bool vkMaterialInstanceAssetLoader::CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData)
{
  bool bTypeID = typeID == "MATERIAL_INSTANCE";
  bool bName = name == "DATA";
  return bTypeID && bName;
}

IObject *vkMaterialInstanceAssetLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData)
{

  vkUInt32 version;
  inputStream >> version;

  vkString materialAsset;
  inputStream >> materialAsset;

  vkMaterialInstance *materialInstance = new vkMaterialInstance();


  vkMaterial *material = vkResourceManager::Get()->GetOrLoad<vkMaterial>(vkResourceLocator(materialAsset), 0);
  if (!material)
  {
    return materialInstance;
  }

  materialInstance->SetMaterial(material);


  //
  // Read all nodes
  vkUInt16 numResources;
  inputStream >> numResources;
  for (vkUInt16 i = 0; i < numResources; ++i)
  {
    vkString parameterName;
    inputStream >> parameterName;

    vkInt16 index = materialInstance->GetIndex(parameterName);
    if (index == -1)
    {
      return materialInstance;
    }

    vkShaderParameterType type = material->GetParamType(index);


    float floats[16];
    int ints[16];
    switch (type)
    {
    case eSPT_Float:
      inputStream >> floats[0];
      materialInstance->Set(index, floats[0]);
      break;
    case eSPT_Vector2:
      inputStream >> floats[0] >> floats[1];
      materialInstance->Set(index, vkVector2f (floats[0], floats[1]));
      break;
    case eSPT_Vector3:
      inputStream >> floats[0] >> floats[1] >> floats[2];
      materialInstance->Set(index, vkVector3f (floats[0], floats[1], floats[2]));
      break;
    case eSPT_Vector4:
      inputStream >> floats[0] >> floats[1] >> floats[2] >> floats[3];
      materialInstance->Set(index, vkVector4f (floats[0], floats[1], floats[2], floats[3]));
      break;
    case eSPT_Int:
      inputStream >> ints[0];
      break;
    case eSPT_IVector2:
      inputStream >> ints[0] >> ints[1];
      break;
    case eSPT_IVector3:
      inputStream >> ints[0] >> ints[1] >> ints[2];
      break;
    case eSPT_IVector4:
      inputStream >> ints[0] >> ints[1] >> ints[2] >> ints[3];
      break;
    case eSPT_Color4:
      inputStream >> floats[0] >> floats[1] >> floats[2] >> floats[3];
      materialInstance->Set(index, vkColor4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case eSPT_Matrix3:
      inputStream
          >> floats[0] >> floats[1] >> floats[2]
          >> floats[3] >> floats[4] >> floats[5]
          >> floats[6] >> floats[7] >> floats[8];
      break;
    case eSPT_Matrix4:
      inputStream
          >> floats[0] >> floats[1] >> floats[2] >> floats[3]
          >> floats[4] >> floats[5] >> floats[6] >> floats[7]
          >> floats[8] >> floats[9] >> floats[10] >> floats[11]
          >> floats[12] >> floats[13] >> floats[14] >> floats[15];
      break;
    case eSPT_Texture:
      {
        vkString loc;
        inputStream >> loc;
        ITexture *texture = vkResourceManager::Get()->GetOrLoad<ITexture>(vkResourceLocator(loc));
        materialInstance->Set(index, texture);
      }
      break;
    }
  }
  return materialInstance;
}

