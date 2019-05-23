
#include <shadergrapheditor/shadergrapheditormetaassetcsfloader.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>




ShaderGraphEditorMetaAssetCSFLoader::ShaderGraphEditorMetaAssetCSFLoader()
  : cs::BaseCSFLoader()
{

}


ShaderGraphEditorMetaAssetCSFLoader::~ShaderGraphEditorMetaAssetCSFLoader()
{

}

bool ShaderGraphEditorMetaAssetCSFLoader::CanLoad(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("shaderGraphMeta");
}

const cs::Class *ShaderGraphEditorMetaAssetCSFLoader::EvalClass(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return ShaderGraphEditorMeta::GetStaticClass();
}

cs::ResourceWrapper *ShaderGraphEditorMetaAssetCSFLoader::Load(const csfEntry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  ShaderGraphEditorMeta *meta = new ShaderGraphEditorMeta();
  ShaderGraphEditorMetaWrapper *metaWrapper = new ShaderGraphEditorMetaWrapper(meta);
  const csfEntry *nodesEntry = entry->GetEntry("nodes");
  if (nodesEntry)
  {
    for (const csfEntry *nodeEntry = nodesEntry->GetEntry("node");
      nodeEntry;
      nodeEntry = nodeEntry->GetSiblingEntry("node"))
    {
      cs::Vector2f pos;
      const csfEntry *posEntry = nodeEntry->GetEntry("pos");
      if (posEntry)
      {
        pos = LoadVector2f(posEntry);
      }
      if (nodeEntry->HasAttribute("id"))
      {
        int id = nodeEntry->GetAttributeInt("id");
        meta->SetPos(id, pos);
      }
    }

    const csfEntry *shaderGraphEntry = entry->GetEntry("shaderGraph");
    if (shaderGraphEntry)
    {
      cs::Vector2f pos;
      const csfEntry *posEntry = shaderGraphEntry->GetEntry("pos");
      if (posEntry)
      {
        pos = LoadVector2f(posEntry);
      }
      meta->SetPos(pos);
    }
  }
  return metaWrapper;
}
