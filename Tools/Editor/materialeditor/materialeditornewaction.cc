
#include <materialeditor/materialeditornewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <csfile/csffile.hh>
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

MaterialEditorNewAction::MaterialEditorNewAction()
{

}

MaterialEditorNewAction::~MaterialEditorNewAction()
{

}

bool MaterialEditorNewAction::ShouldShow(AssetManagerWidget *assetManager) const
{
  return true;
}

bool MaterialEditorNewAction::IsEnabled(AssetManagerWidget *assetManager) const
{
  return true;
}

QString MaterialEditorNewAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
  return QString("New Material");
}

csResourceLocator MaterialEditorNewAction::CreateNewAsset(AssetManagerWidget *assetManager) const
{
  /*
  QString assetName = assetManager->GetNewAssetName("Material");
  if (assetName == QString::null)
  {
    return csResourceLocator();
  }

  csResourceLocator contentLocator = assetManager->GetContentResource((const char*)assetName.toLatin1());

  QString assetFilePath = assetManager->GetFilePath(assetName);
  QFile file(assetFilePath);
  if (!file.open(QIODevice::WriteOnly))
  {
    return csResourceLocator();
  }

  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement materialElement = doc.createElement("material");
  QDomElement materialDefElement = doc.createElement("materialDef");
  QDomElement parametersElement = doc.createElement("parameters");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(materialElement);
  materialElement.appendChild(materialDefElement);
  materialElement.appendChild(parametersElement);

  QString content = doc.toString(2);
  file.write(content.toLatin1());
  file.close();
  */

  QString assetName = assetManager->GetNewAssetName("Material");
  if (assetName == QString::null)
  {
    return csResourceLocator();
  }

  assetName = assetManager->GetFilePath(assetName);
  csResourceLocator contentLocator = assetManager->GetContentResource((const char*)assetName.toLatin1());


  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *materialEntry = outputFile.CreateEntry("material");
  csfEntry *materialDefEntry = outputFile.CreateEntry("materialDef");
  csfEntry *parametersEntry = outputFile.CreateEntry("parameters");

  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(materialEntry);
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

  outputFile.Output(std::string(assetName.toLatin1()));
  return contentLocator;
}
