
#include <editors/shadergrapheditor/shadergrapheditormetaassetxmlloader.hh>
#include <editors/shadergrapheditor/shadergrapheditormeta.hh>




ShaderGraphEditorMetaAssetXMLLoader::ShaderGraphEditorMetaAssetXMLLoader()
  : csBaseXMLLoader()
{

}


ShaderGraphEditorMetaAssetXMLLoader::~ShaderGraphEditorMetaAssetXMLLoader()
{

}

bool ShaderGraphEditorMetaAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  std::string tagName(element->Value());
  return tagName == std::string("shaderGraphMeta");
}

const csClass *ShaderGraphEditorMetaAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return ShaderGraphEditorMeta::GetStaticClass();
}

iObject *ShaderGraphEditorMetaAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  ShaderGraphEditorMeta *meta = new ShaderGraphEditorMeta();
  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  if (nodesElement)
  {
    for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node");
      nodeElement;
      nodeElement = nodeElement->NextSiblingElement("node"))
    {
      csVector2f pos;
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
      csVector2f pos;
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
