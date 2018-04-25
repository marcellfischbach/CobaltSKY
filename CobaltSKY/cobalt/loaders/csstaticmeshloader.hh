#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csstaticmeshloader.refl.hh>

class csGeometryData;
class csGeometryMesh;
class csMesh;
class csMultiMaterial;
class csPhysicsShape;
class csSubMesh;
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

  virtual bool CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  virtual iObject *Load(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;

private:
  struct HeaderEntry
  {
    std::string name;
    csUInt32 type;
    csUInt32 position;
    csUInt32 size;
    iObject *obj;
  };

  iObject *ReadEntry(std::map<std::string, HeaderEntry> &entries, const std::string &entryName, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  csGeometryData *ReadGeometry(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  csGeometryMesh *ReadGeometryMesh(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  csMultiMaterial *ReadMultiMaterial(iFile *file) const;
  csMesh *ReadMesh (csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  csPhysicsShape *ReadCollision(csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  bool ReadSubMesh(csMesh *mesh, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;
  iVertexDeclaration *ReadVertexDeclaration(iFile *file) const;

  std::string ReadString(iFile *file) const;
  
};

