

#include <MaterialInstance/CreateNewMaterialInstance.hh>
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/VFS.hh>

namespace materialinstance
{




NewMaterialInstanceAction::NewMaterialInstanceAction(QObject *parent)
  : assetmanager::NewAssetAction(tr ("New Material Instance"), parent)
{

}

bool NewMaterialInstanceAction::activate()
{
  //vkAssetOutputStream osMeta;
  //osMeta << (vkUInt32)VK_VERSION(1, 0, 0);

  vkAssetOutputStream osData;
  osData << (vkUInt32)VK_VERSION(1, 0, 0)
         << vkString("")
         << (vkUInt16)0;

  IFile *file = vkVFS::Get()->Open(GetLocator().GetResourceFile(), eOM_Write, eTM_Binary);
  assetmanager::AssetWriter writer;
  writer.AddEntry("MATERIAL_INSTANCE", "DATA", osData.GetSize(), osData.GetBuffer());
  //writer.AddEntry("MATERIAL_INSTANCE", "META_DATA", osMeta.GetSize(), osMeta.GetBuffer());

  writer.Output(file);
  file->Close();

  return true;
}



}
