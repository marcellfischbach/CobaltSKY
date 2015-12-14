#pragma once



#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/StaticMeshLoader.refl.hh>

class vkMesh;
class vkSubMesh;
struct IVertexDeclaration;

VK_CLASS();
class VKE_API vkStaticMeshLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;

public:
  vkStaticMeshLoader();
  virtual ~vkStaticMeshLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

private:
  vkMesh *ReadMesh (vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  vkSubMesh *ReadSubMesh(vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  IVertexDeclaration *ReadVertexDeclaration(IFile *file) const;

  vkString ReadString(IFile *file) const;
  
};