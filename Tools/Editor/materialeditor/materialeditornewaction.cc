
#include <materialeditor/materialeditornewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
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

bool MaterialEditorNewAction::PerformAction(AssetManagerWidget *assetManager) const
{
  QString assetName = assetManager->GetNewAssetName("Material");
  if (assetName == QString::null)
  {
    return false;
  }

  QString assetFilePath = assetManager->GetFilePath(assetName);
  QFile file(assetFilePath);
  if (!file.open(QIODevice::WriteOnly))
  {
    return false;
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
  return true;
}