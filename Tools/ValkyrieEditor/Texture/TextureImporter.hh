#pragma once

#include <qstring.h>

#include <AssetManager/Importer.hh>

namespace texture
{

class Importer : public assetmanager::Importer
{

public:
  static Importer *Get();

  virtual bool CanImport(const QFileInfo &info);

  virtual bool Import(const QFileInfo &info, const QDir &outputDir);


private:
  Importer () { }
  virtual ~Importer() { }
};

}