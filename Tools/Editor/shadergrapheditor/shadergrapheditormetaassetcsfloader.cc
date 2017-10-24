
#include <shadergrapheditor/shadergrapheditormetaassetcsfloader.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>




ShaderGraphEditorMetaAssetCSFLoader::ShaderGraphEditorMetaAssetCSFLoader()
  : csBaseCSFLoader()
{

}


ShaderGraphEditorMetaAssetCSFLoader::~ShaderGraphEditorMetaAssetCSFLoader()
{

}

bool ShaderGraphEditorMetaAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("shaderGraphMeta");
}

const csClass *ShaderGraphEditorMetaAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return ShaderGraphEditorMeta::GetStaticClass();
}

iObject *ShaderGraphEditorMetaAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  ShaderGraphEditorMeta *meta = new ShaderGraphEditorMeta();
  const csfEntry *nodesEntry = entry->GetEntry("nodes");
  if (nodesEntry)
  {
    for (const csfEntry *nodeEntry = nodesEntry->GetEntry("node");
      nodeEntry;
      nodeEntry = nodeEntry->GetSiblingEntry("node"))
    {
      csVector2f pos;
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
      csVector2f pos;
      const csfEntry *posEntry = shaderGraphEntry->GetEntry("pos");
      if (posEntry)
      {
        pos = LoadVector2f(posEntry);
      }
      meta->SetPos(pos);
    }
  }
  return meta;
}
