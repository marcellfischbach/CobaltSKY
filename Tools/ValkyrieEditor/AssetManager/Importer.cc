
#include <AssetManager/Importer.hh>


namespace assetmanager
{

ImporterRegistry::ImporterRegistry()
{
  m_importers.clear();
}

ImporterRegistry *ImporterRegistry::Get()
{
  static ImporterRegistry reg;
  return &reg;
}

void ImporterRegistry::RegisterImporter(Importer *importer)
{
  m_importers.append(importer);
}

Importer *ImporterRegistry::GetImporter(const QFileInfo &info)
{
  for (Importer *importer : m_importers)
  {
    if (importer->CanImport(info))
    {
      return importer;
    }
  }
  return 0;
}

}