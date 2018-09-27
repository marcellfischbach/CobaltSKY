
#include <imageimporter/imageimporterfactory.hh>
#include <imageimporter/imageimporter.hh>


ImageImporterFactory::ImageImporterFactory()
{

}


const std::string ImageImporterFactory::GetName() const
{
  return std::string("Images");
}

const std::vector<std::string> ImageImporterFactory::GetExtensions() const
{
  std::vector<std::string> extensions;
  extensions.push_back("png");
  extensions.push_back("jpeg");
  extensions.push_back("jpg");
  extensions.push_back("bmp");
  extensions.push_back("tif");
  extensions.push_back("tiff");
  return extensions;
}

bool ImageImporterFactory::CanImport(const std::filesystem::path &path) const
{
  return true;
}

std::vector<iAssetImporter*> ImageImporterFactory::Import(const std::filesystem::path &path) const
{
  std::vector<iAssetImporter*> importers;
  importers.push_back(new ImageImporter(path));
  return importers;
}
