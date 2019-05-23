
#include <shadergrapheditor/shadergrapheditormetaassetcsfloader.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>




ShaderGraphEditorMetaAssetCSFLoader::ShaderGraphEditorMetaAssetCSFLoader()
  : cs::BaseCSFLoader()
{

}


ShaderGraphEditorMetaAssetCSFLoader::~ShaderGraphEditorMetaAssetCSFLoader()
{

}

bool ShaderGraphEditorMetaAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("shaderGraphMeta");
}

const cs::Class *ShaderGraphEditorMetaAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return ShaderGraphEditorMeta::GetStaticClass();
}

cs::ResourceWrapper *ShaderGraphEditorMetaAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  ShaderGraphEditorMeta *meta = new ShaderGraphEditorMeta();
  ShaderGraphEditorMetaWrapper *metaWrapper = new ShaderGraphEditorMetaWrapper(meta);
  const cs::file::Entry *nodesEntry = entry->GetEntry("nodes");
  if (nodesEntry)
  {
    for (const cs::file::Entry *nodeEntry = nodesEntry->GetEntry("node");
      nodeEntry;
      nodeEntry = nodeEntry->GetSiblingEntry("node"))
    {
      cs::Vector2f pos;
      const cs::file::Entry *posEntry = nodeEntry->GetEntry("pos");
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

    const cs::file::Entry *shaderGraphEntry = entry->GetEntry("shaderGraph");
    if (shaderGraphEntry)
    {
      cs::Vector2f pos;
      const cs::file::Entry *posEntry = shaderGraphEntry->GetEntry("pos");
      if (posEntry)
      {
        pos = LoadVector2f(posEntry);
      }
      meta->SetPos(pos);
    }
  }
  return metaWrapper;
}
