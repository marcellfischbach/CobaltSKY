#pragma once

#include <AssetManager/Importer.hh>
#include <Valkyrie/Core/AssetStream.hh>


class vkMatrix4f;
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

  void Write(vkAssetOutputStream &outputStream, const vkMatrix4f &matrix, const vkMatrix4f &normalMatrix, aiMesh *mesh);
};

}