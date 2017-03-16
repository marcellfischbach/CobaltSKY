
#include <shadergrapheditor/shadergrapheditormetaassetxmlloader.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>




ShaderGraphEditorMetaAssetXMLLoader::ShaderGraphEditorMetaAssetXMLLoader()
  : vkBaseXMLLoader()
{

}


ShaderGraphEditorMetaAssetXMLLoader::~ShaderGraphEditorMetaAssetXMLLoader()
{

}

bool ShaderGraphEditorMetaAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("shaderGraphMeta");
}

const vkClass *ShaderGraphEditorMetaAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return ShaderGraphEditorMeta::GetStaticClass();
}

iObject *ShaderGraphEditorMetaAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  ShaderGraphEditorMeta *meta = new ShaderGraphEditorMeta();
  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  if (nodesElement)
  {
    for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node");
      nodeElement;
      nodeElement = nodeElement->NextSiblingElement("node"))
    {
      vkVector2f pos;
      TiXmlElement *posElement = nodeElement->FirstChildElement("pos");
      if (posElement)
      {
        pos = LoadVector2f(posElement->GetText());
      }
      if (nodeElement->Attribute("id"))
      {
        int id = atoi(nodeElement->Attribute("id"));
        meta->SetPos(id, pos);
      }
    }

    TiXmlElement *shaderGraphElement = element->FirstChildElement("shaderGraph");
    if (shaderGraphElement)
    {
      vkVector2f pos;
      TiXmlElement *posElement = shaderGraphElement->FirstChildElement("pos");
      if (posElement)
      {
        pos = LoadVector2f(posElement->GetText());
      }
      meta->SetPos(pos);
    }
  }
  return meta;
}
