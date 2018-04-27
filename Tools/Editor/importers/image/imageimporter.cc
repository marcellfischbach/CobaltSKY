
#include <importers/image/imageimporter.hh>
#include <QString>


ImageImporterGUI::ImageImporterGUI()
  : QWidget()
{
  m_gui.setupUi(this);
}


newImageImporter::newImageImporter(const std::string &fileName)
  : m_fileName(fileName)
{
  m_gui = new ImageImporterGUI();
}

std::string newImageImporter::GetName () const
{
  return "Image";
}

std::string newImageImporter::GetAssetName () const
{
  return m_fileName;
}

QWidget *newImageImporter::GetWidget ()
{
  return m_gui;
}




bool ImageImporterFactory::CanImport(const std::string &fileName) const
{
//  QString name (fileName);
  return true;
}


std::map<std::string, std::vector<std::string>> ImageImporterFactory::GetImportFormats() const
{
  std::map<std::string, std::vector<std::string>> result;

  result["JPEG Image file"].push_back("*.jpg");
  result["JPEG Image file"].push_back("*.jpeg");
  result["PNG Image file"].push_back("*.png");
  result["BMP Image file"].push_back("*.bmp");
  return result;
}


iImporter *ImageImporterFactory::CreateImporter (const std::string &fileName) const
{
  return new newImageImporter(fileName);
}
