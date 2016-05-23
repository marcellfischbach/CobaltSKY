#pragma once



#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/StaticMeshLoader.refl.hh>

class vkGeometryData;
class vkGeometryMesh;
class vkMesh;
class vkMultiMaterial;
class vkPhysicsShapeContainer;
class vkSubMesh;
struct IVertexDeclaration;
struct IIndexBuffer;
/**
* \ingroup loading
*/
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
  struct HeaderEntry
  {
    vkString name;
    vkUInt32 type;
    vkUInt32 position;
    vkUInt32 size;
    IObject *obj;
  };

  IObject *ReadEntry(std::map<vkString, HeaderEntry> &entries, const vkString &entryName, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  vkGeometryData *ReadGeometry(std::map<vkString, HeaderEntry> &entries, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  vkGeometryMesh *ReadGeometryMesh(std::map<vkString, HeaderEntry> &entries, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  vkMultiMaterial *ReadMultiMaterial(IFile *file) const;
  vkMesh *ReadMesh (vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  vkPhysicsShapeContainer *ReadCollision(vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  bool ReadSubMesh(vkMesh *mesh, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  IVertexDeclaration *ReadVertexDeclaration(IFile *file) const;

  vkString ReadString(IFile *file) const;
  
};


VK_CLASS()
class VKE_API vkStaticMeshAssetLoader : public IAssetLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkStaticMeshAssetLoader();
  virtual ~vkStaticMeshAssetLoader();

  virtual bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0);

  IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0);
  
private:
  vkSubMesh *ReadSubMesh(vkAssetInputStream &inputStream, std::vector<IIndexBuffer*> &globalIndexBuffers, const vkResourceLocator &locator, IObject *userData = 0);
};