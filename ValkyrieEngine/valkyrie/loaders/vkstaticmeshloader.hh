#pragma once



#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkstaticmeshloader.refl.hh>

class vkGeometryData;
class vkGeometryMesh;
class vkMesh;
class vkMultiMaterial;
class vkPhysicsShapeContainer;
class vkSubMesh;
struct iVertexDeclaration;
struct iIndexBuffer;
/**
* \ingroup loading
*/
VK_CLASS()
class VKE_API vkStaticMeshLoader : public VK_SUPER(iFileLoader)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkStaticMeshLoader();
  virtual ~vkStaticMeshLoader();

  virtual bool CanLoad(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;

private:
  struct HeaderEntry
  {
    vkString name;
    vkUInt32 type;
    vkUInt32 position;
    vkUInt32 size;
    iObject *obj;
  };

  iObject *ReadEntry(std::map<vkString, HeaderEntry> &entries, const vkString &entryName, vkUInt32 fileVersion, iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  vkGeometryData *ReadGeometry(std::map<vkString, HeaderEntry> &entries, vkUInt32 fileVersion, iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  vkGeometryMesh *ReadGeometryMesh(std::map<vkString, HeaderEntry> &entries, vkUInt32 fileVersion, iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  vkMultiMaterial *ReadMultiMaterial(iFile *file) const;
  vkMesh *ReadMesh (vkUInt32 fileVersion, iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  vkPhysicsShapeContainer *ReadCollision(vkUInt32 fileVersion, iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  bool ReadSubMesh(vkMesh *mesh, vkUInt32 fileVersion, iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;
  iVertexDeclaration *ReadVertexDeclaration(iFile *file) const;

  vkString ReadString(iFile *file) const;
  
};

