#pragma once

#include <qfileinfo.h>
#include <qlist.h>

namespace assetmanager
{

struct Importer
{
  virtual bool CanImport(const QFileInfo &info) = 0;

  virtual bool Import(const QFileInfo &info, const QDir &outputDir) = 0;

};
class ImporterRegistry
{
public:
  static ImporterRegistry *Get();

  void RegisterImporter(Importer *importer);

  Importer *GetImporter(const QFileInfo &info);

private:
  ImporterRegistry();

  QList<Importer*> m_importers;
};
}