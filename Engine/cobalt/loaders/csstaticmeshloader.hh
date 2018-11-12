#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csstaticmeshloader.refl.hh>

class csGeometryDataWrapper;
class csGeometryMeshWrapper;
class csMesh;
class csMeshWrapper;
class csMultiMaterial;
class csPhysicsShapeWrapper;
class csSubMeshWrapper;
struct iVertexDeclaration;
struct iIndexBuffer;
/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API csStaticMeshLoader : public CS_SUPER(iFileLoader)
{
  CS_CLASS_GEN_OBJECT;

public:
  csStaticMeshLoader();
  virtual ~csStaticMeshLoader();

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  virtual csResourceWrapper *Load(iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;

private:
  struct HeaderEntry
  {
    std::string name;
    csUInt32 type;
    csUInt32 position;
    csUInt32 size;
    csResourceWrapper *obj;
  };

  csResourceWrapper *ReadEntry(std::map<std::string, HeaderEntry> &entries, const std::string &entryName, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  csGeometryDataWrapper *ReadGeometry(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  csGeometryMeshWrapper *ReadGeometryMesh(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  csMultiMaterial *ReadMultiMaterial(iFile *file) const;
  csMeshWrapper *ReadMesh (csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  csPhysicsShapeWrapper *ReadCollision(csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  bool ReadSubMesh(csMesh *mesh, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = nullptr) const;
  iVertexDeclaration *ReadVertexDeclaration(iFile *file) const;

  std::string ReadString(iFile *file) const;
  
};

