
#include <importers/model/modelimporter.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <cobalt/core/csfileinfo.hh>


ModelImporterGUI::ModelImporterGUI()
  : QWidget()
{
  m_gui.setupUi(this);
}



newModelImporter::newModelImporter(const std::string &fileName)
  : m_fileName(fileName)
{
  m_gui = new ModelImporterGUI();
  csFileInfo fileInfo (m_fileName);
  m_assetName = fileInfo.GetName() + "." + fileInfo.GetExtension();

}

std::string newModelImporter::GetName () const
{
  return "Model";
}

std::string newModelImporter::GetAssetName () const
{
  return m_assetName;
}

std::string newModelImporter::GetFileName () const
{
  return m_fileName;
}

QWidget *newModelImporter::GetWidget ()
{
  return m_gui;
}


bool newModelImporter::Import(AssetManagerWidget *assetManager)
{

  return true;
}


std::string newModelImporter::GetType() const
{
  csFileInfo fi(m_fileName);
  fi.Upper();
  return fi.GetExtension().c_str();
}


bool ModelImporterFactory::CanImport(const std::string &fileName) const
{
  csFileInfo fi(fileName);
  std::string ext = fi.GetExtension();
  return ext == std::string("fbx")
      || ext == std::string("dae");
}


std::map<std::string, std::vector<std::string>> ModelImporterFactory::GetImportFormats() const
{
  std::map<std::string, std::vector<std::string>> result;

  result["Autodesk FilmBox"].push_back("*.fbx");
  result["COLLADA"].push_back("*.dae");
  return result;
}


iImporter *ModelImporterFactory::CreateImporter (const std::string &fileName) const
{
  return new newModelImporter(fileName);
}
