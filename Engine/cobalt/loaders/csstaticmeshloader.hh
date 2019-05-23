#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csstaticmeshloader.refl.hh>


namespace cs
{
struct iIndexBuffer;
struct iVertexDeclaration;
class GeometryMeshWrapper;
class GeometryDataWrapper;
class Mesh;
class MeshWrapper;
class MultiMaterial;
class PhysicsShapeWrapper;
class SubMeshWrapper;

/**
* \ingroup loading
*/
CS_CLASS()
class CSE_API StaticMeshLoader : public CS_SUPER(cs::iFileLoader)
{
  CS_CLASS_GEN_OBJECT;

public:
  StaticMeshLoader();
  virtual ~StaticMeshLoader();

  virtual bool CanLoad(cs::iFile *file, const cs::ResourceLocator &locator) const;
  virtual const cs::Class *EvalClass(cs::iFile *file, const cs::ResourceLocator &locator) const;
  virtual cs::ResourceWrapper *Load(cs::iFile *file, const cs::ResourceLocator &locator) const;

private:
  struct HeaderEntry
  {
    std::string name;
    csUInt32 type;
    csUInt32 position;
    csUInt32 size;
    cs::ResourceWrapper *obj;
  };

  cs::ResourceWrapper *ReadEntry(std::map<std::string, HeaderEntry> &entries, const std::string &entryName, csUInt32 fileVersion, cs::iFile *file, const cs::ResourceLocator &locator) const;
  cs::GeometryDataWrapper *ReadGeometry(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, cs::iFile *file, const cs::ResourceLocator &locator) const;
  cs::GeometryMeshWrapper *ReadGeometryMesh(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, cs::iFile *file, const cs::ResourceLocator &locator) const;
  cs::MultiMaterial *ReadMultiMaterial(cs::iFile *file) const;
  cs::MeshWrapper *ReadMesh (csUInt32 fileVersion, cs::iFile *file, const cs::ResourceLocator &locator) const;
  cs::PhysicsShapeWrapper *ReadCollision(csUInt32 fileVersion, cs::iFile *file, const cs::ResourceLocator &locator) const;
  bool ReadSubMesh(cs::Mesh *mesh, csUInt32 fileVersion, cs::iFile *file, const cs::ResourceLocator &locator) const;
  cs::iVertexDeclaration *ReadVertexDeclaration(cs::iFile *file) const;

  std::string ReadString(cs::iFile *file) const;
  
};


}

