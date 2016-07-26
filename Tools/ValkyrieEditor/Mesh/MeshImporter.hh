#pragma once

#include <AssetManager/Importer.hh>
#include <Valkyrie/Core/AssetStream.hh>


struct vkMatrix4f;
struct aiMesh;

namespace mesh
{

class Importer : public assetmanager::Importer
{

public:
  static Importer *Get();

  virtual bool CanImport(const QFileInfo &info);
  virtual bool Import(const QFileInfo &info, const QDir &outputDir);


private:
  Importer() { }
  virtual ~Importer() { }

  

};

}