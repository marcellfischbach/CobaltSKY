
#include <shadergrapheditor/shadergrapheditornewhandler.hh>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>

ShaderGraphEditorNewHandler::ShaderGraphEditorNewHandler()
{

}

ShaderGraphEditorNewHandler::~ShaderGraphEditorNewHandler()
{

}

QString ShaderGraphEditorNewHandler::GetTypeName() const
{
  return "Material";
}

bool ShaderGraphEditorNewHandler::CreateNewAsset(const QDir &dir, const QString &assetName)
{
  QString filePath = dir.absoluteFilePath(assetName + ".xasset");
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly))
  {
    return false;
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
  return true;
}