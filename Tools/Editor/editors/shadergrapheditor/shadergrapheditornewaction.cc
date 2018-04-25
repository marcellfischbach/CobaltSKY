
#include <editors/shadergrapheditor/shadergrapheditornewaction.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <csfile/csffile.hh>
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
  csfFile outputFile;

  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *shaderGraphEntry = outputFile.CreateEntry("shaderGraph");
  csfEntry *nodesEntry = outputFile.CreateEntry("nodes");
  csfEntry *inputsEntry = outputFile.CreateEntry("inputs");
  csfEntry *attributesEntry = outputFile.CreateEntry("attributes");

  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(shaderGraphEntry);
  shaderGraphEntry->AddChild(nodesEntry);
  shaderGraphEntry->AddChild(inputsEntry);
  shaderGraphEntry->AddChild(attributesEntry);

  outputFile.Output(std::string(assetFilePath.toLatin1()));
  return contentLocator;
}
