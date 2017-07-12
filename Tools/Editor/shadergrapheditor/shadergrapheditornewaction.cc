
#include <shadergrapheditor/shadergrapheditornewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

ShaderGraphEditorNewAction::ShaderGraphEditorNewAction()
{

}

ShaderGraphEditorNewAction::~ShaderGraphEditorNewAction()
{

}

bool ShaderGraphEditorNewAction::ShouldShow(AssetManagerWidget *assetManager) const
{
  return true;
}

bool ShaderGraphEditorNewAction::IsEnabled(AssetManagerWidget *assetManager) const
{
  return true;
}

QString ShaderGraphEditorNewAction::GetMenuEntryName(AssetManagerWidget *assetManager) const
{
  return QString("New MaterialDef");
}

csResourceLocator ShaderGraphEditorNewAction::CreateNewAsset(AssetManagerWidget *assetManager) const
{
  QString assetName = assetManager->GetNewAssetName("ShaderGraph");
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
  QDomElement shaderGraphElement = doc.createElement("shaderGraph");
  QDomElement nodesElements = doc.createElement("nodes");
  QDomElement inputsElements = doc.createElement("inputs");
  QDomElement attributesElements = doc.createElement("attributes");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(shaderGraphElement);
  shaderGraphElement.appendChild(nodesElements);
  shaderGraphElement.appendChild(inputsElements);
  shaderGraphElement.appendChild(attributesElements);

  QString content = doc.toString(2);
  file.write(content.toLatin1());
  file.close();
  return contentLocator;
}