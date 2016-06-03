

#include <ShaderGraph/CreateNewShaderGraph.hh>
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/VFS.hh>

namespace shadergraph
{

CreateNewShaderGraph::CreateNewShaderGraph()
{

}


void CreateNewShaderGraph::CreateNew(const vkResourceLocator &locator)
{

  vkAssetOutputStream osMeta;
  osMeta << (vkUInt32)VK_VERSION(1, 0, 0);
  osMeta << vkVector2f(0.0f, 0.0f);

  vkAssetOutputStream osData;
  osData << (vkUInt32)VK_VERSION(1, 0, 0);
  osData << (vkUInt16)(0); // no node in a clean shader
  osMeta << (vkUInt16)(0);
  osData << (vkUInt16)0; // no input
  osData << (vkUInt16)0;

  osData << (vkUInt16)2
            // binary gradient
         << vkString("blendOutWithBinaryGradient")
         << (vkUInt8)(0)
            // discard alpha
         << vkString("discardAlpha")
         << (vkUInt8)(0)
         << (vkUInt8)(eCM_Less)
         << (float)0.0f;


  IFile *file = vkVFS::Get()->Open(locator.GetResourceFile(), eOM_Write, eTM_Binary);
  assetmanager::AssetWriter writer;
  writer.AddEntry("SHADER_GRAPH", "DATA", osData.GetSize(), osData.GetBuffer());
  writer.AddEntry("SHADER_GRAPH", "META_DATA", osMeta.GetSize(), osMeta.GetBuffer());

  writer.Output(file);
  file->Close();

}



NewShaderGraphAction::NewShaderGraphAction(QObject *parent)
  : assetmanager::NewAssetAction(tr ("New Material"), parent)
{

}

bool NewShaderGraphAction::activate()
{
  vkAssetOutputStream osMeta;
  osMeta << (vkUInt32)VK_VERSION(1, 0, 0);
  osMeta << vkVector2f(0.0f, 0.0f);

  vkAssetOutputStream osData;
  osData << (vkUInt32)VK_VERSION(1, 0, 0);
  osData << (vkUInt16)(0); // no node in a clean shader
  osMeta << (vkUInt16)(0);
  osData << (vkUInt16)0; // no input
  osData << (vkUInt16)0;

  osData << (vkUInt16)2
            // binary gradient
         << vkString("blendOutWithBinaryGradient")
         << (vkUInt8)(0)
            // discard alpha
         << vkString("discardAlpha")
         << (vkUInt8)(0)
         << (vkUInt8)(eCM_Less)
         << (float)0.0f;



  IFile *file = vkVFS::Get()->Open(GetLocator().GetResourceFile(), eOM_Write, eTM_Binary);
  assetmanager::AssetWriter writer;
  writer.AddEntry("SHADER_GRAPH", "DATA", osData.GetSize(), osData.GetBuffer());
  writer.AddEntry("SHADER_GRAPH", "META_DATA", osMeta.GetSize(), osMeta.GetBuffer());

  writer.Output(file);
  file->Close();

  return true;
}



}
