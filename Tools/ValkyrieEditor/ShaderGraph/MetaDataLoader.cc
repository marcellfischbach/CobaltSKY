
#include <ShaderGraph/MetaDataLoader.hh>
#include <ShaderGraph/MetaData.hh>

MetaDataLoader::MetaDataLoader()
  : IAssetLoader()
{

}

MetaDataLoader::~MetaDataLoader()
{

}



bool MetaDataLoader::CanLoad(const vkString &typeID,const vkResourceLocator &locator, IObject *userData)
{
  bool bTypeID = typeID == "SHADER_GRAPH_META";
  return bTypeID;
}


IObject *MetaDataLoader::Load(vkAssetInputStream &is, const vkResourceLocator &locator, IObject *userData)
{
  ShaderGraphMetaData *metaData = new ShaderGraphMetaData();
  vkUInt32 version;
  is >> version;

  vkVector2f pos;
  is >> pos;
  metaData->SetGraphPosition(pos);

  vkUInt16 numNodes;
  vkUInt32 idx;
  is >> numNodes;
  for (vkUInt32 i = 0; i < numNodes; ++i)
  {
    is >> idx >> pos;
    metaData->SetNodePosition(idx, pos);
  }
  return metaData;
}

