#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/MeshLoader.refl.hh>


VK_CLASS()
class VKE_API vkMeshAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkMeshAssetXMLLoader();
  virtual ~vkMeshAssetXMLLoader();


  bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};

VK_CLASS()
class VKE_API vkSkinnedMeshAssetXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkSkinnedMeshAssetXMLLoader();
  virtual ~vkSkinnedMeshAssetXMLLoader();


  bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

};

VK_CLASS()
class VKE_API vkSubMeshAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkSubMeshAssetLoader();
  virtual ~vkSubMeshAssetLoader();

  bool CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData = 0)const;
  virtual const vkClass *EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;
};

