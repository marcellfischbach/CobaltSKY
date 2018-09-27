
#include <imageimporter/imageimporterexport.hh>
#include <editor/iassetimporterfactory.hh>

class ImageImporterFactory : public iAssetImporterFactory
{
public:
  ImageImporterFactory();

  virtual ~ImageImporterFactory() { }

  virtual const std::string GetName() const;

  virtual const std::vector<std::string> GetExtensions() const;

  virtual bool CanImport(const std::filesystem::path &path) const;

  virtual std::vector<iAssetImporter*> Import(const std::filesystem::path &path) const;
};

