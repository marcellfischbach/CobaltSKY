

#include <ShaderGraph/CreateNewShaderGraph.hh>
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/VFS.hh>
#include <qdom.h>

namespace shadergraph
{


NewShaderGraphAction::NewShaderGraphAction(QObject *parent)
  : assetmanager::NewAssetAction(tr ("New Material"), parent)
{

}

bool NewShaderGraphAction::activate()
{

  QDomDocument doc;

  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement shaderGraphElement = doc.createElement("shaderGraph");
  QDomElement nodesElement = doc.createElement("nodes");
  QDomElement inputsElement = doc.createElement("inputs");
  QDomElement attributesElement = doc.createElement("attributes");
  QDomElement metaElement = doc.createElement("meta");
  QDomElement shaderGraphMetaElement = doc.createElement("shaderGraphMeta");
  QDomElement shaderGraphMetaNodesElement = doc.createElement("nodes");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  assetElement.appendChild(metaElement);

  dataElement.appendChild(shaderGraphElement);

  shaderGraphElement.appendChild(nodesElement);
  shaderGraphElement.appendChild(inputsElement);
  shaderGraphElement.appendChild(attributesElement);

  metaElement.appendChild(shaderGraphMetaElement);
  shaderGraphMetaElement.appendChild(shaderGraphMetaNodesElement);

  QString xml = doc.toString(2);

  IFile *file = vkVFS::Get()->Open(GetLocator().GetResourceFile(), eOM_Write, eTM_Text);
  file->Write((const char*)xml.toLatin1(), xml.length());
  file->Close();

  return true;
}



}
