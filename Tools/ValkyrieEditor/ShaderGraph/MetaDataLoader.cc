
#include <ShaderGraph/MetaDataLoader.hh>
#include <ShaderGraph/MetaData.hh>

MetaDataLoader::MetaDataLoader()
  : IAssetLoader()
{

}

MetaDataLoader::~MetaDataLoader()
{

}



bool MetaDataLoader::CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData) const
{
  bool bTypeID = typeID == "SHADER_GRAPH";
  bool bName = name == "META_DATA";
  return bTypeID && bName;
}


IObject *MetaDataLoader::Load(vkAssetInputStream &is, const vkResourceLocator &locator, IObject *userData) const
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

