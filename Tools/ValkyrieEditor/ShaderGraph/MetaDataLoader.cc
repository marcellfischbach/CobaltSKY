
#include <ShaderGraph/MetaDataLoader.hh>
#include <ShaderGraph/MetaData.hh>

MetaDataLoader::MetaDataLoader()
  : IAssetLoader()
{

}

MetaDataLoader::~MetaDataLoader()
{

}



bool MetaDataLoader::CanLoad(const vkString &typeID,const vkResourceLocator &locator, IObject *userData) const
{
  bool bTypeID = typeID == "SHADER_GRAPH_META";
  return bTypeID;
}


const vkClass *MetaDataLoader::EvalClass(vkAssetInputStream &is, const vkResourceLocator &locator, IObject *userData) const
{
  return ShaderGraphMetaData::GetStaticClass();
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



ShaderGraphMetaDataAssetXMLLoader::ShaderGraphMetaDataAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

ShaderGraphMetaDataAssetXMLLoader::~ShaderGraphMetaDataAssetXMLLoader()
{

}

bool ShaderGraphMetaDataAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("shadergraphmeta");
}

const vkClass *ShaderGraphMetaDataAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return ShaderGraphMetaData::GetStaticClass();
}

IObject *ShaderGraphMetaDataAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  ShaderGraphMetaData *metaData = new ShaderGraphMetaData();
  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  if (nodesElement)
  {
    for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node"); nodeElement; nodeElement = nodeElement->NextSiblingElement("node"))
    {
      if (!nodeElement->Attribute("id"))
      {
        continue;
      }
      vkUInt32 id = atoi(nodeElement->Attribute("id"));
      TiXmlElement *posElement = nodeElement->FirstChildElement("pos");
      if (posElement)
      {
        vkVector2f pos = LoadVector2f(posElement->GetText());
        metaData->SetNodePosition(id, pos);
      }
    }
  }

  return metaData;
}
