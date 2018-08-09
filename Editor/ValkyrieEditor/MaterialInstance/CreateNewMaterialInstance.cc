

#include <MaterialInstance/CreateNewMaterialInstance.hh>
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/VFS.hh>
#include <qdom.h>

namespace materialinstance
{




NewMaterialInstanceAction::NewMaterialInstanceAction(QObject *parent)
  : assetmanager::NewAssetAction(tr ("New Material Instance"), parent)
{

}

bool NewMaterialInstanceAction::activate()
{

  QDomDocument doc;

  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement materialInstanceElement = doc.createElement("materialInstance");
  QDomElement metaElement = doc.createElement("meta");
  QDomElement materialInstanceMetaElement = doc.createElement("materialInstanceMeta");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  assetElement.appendChild(metaElement);

  dataElement.appendChild(materialInstanceElement);


  metaElement.appendChild(materialInstanceMetaElement);

  QString xml = doc.toString(2);

  IFile *file = vkVFS::Get()->Open(GetLocator().GetResourceFile(), eOM_Write, eTM_Text);
  file->Write((const char*)xml.toLatin1(), xml.length());
  file->Close();


  return true;
}



}
