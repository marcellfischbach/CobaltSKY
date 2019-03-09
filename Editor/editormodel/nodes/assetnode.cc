#include <editormodel/nodes/assetnode.hh>

namespace cs::editor::model
{


AssetNode::AssetNode(Model *model)
  : Node (model, eT_Asset)
{

}

AssetNode::~AssetNode()
{

}

bool AssetNode::IsAssetNode() const
{
  return true;
}

AssetNode *AssetNode::AsAssetNode()
{
  return this;
}

const AssetNode *AssetNode::AsAssetNode() const
{
  return this;
}


void AssetNode::SetName(const std::string &name)
{
  m_name = name;
  UpdateResourceLocator();
}

const std::string &AssetNode::GetName() const
{
  return m_name;
}

void AssetNode::SetAssetTypeName(const std::string &assetTypeName)
{
  m_assetTypeName = assetTypeName;
}

const std::string &AssetNode::GetAssetTypeName() const
{
  return m_assetTypeName;
}


std::set<csResourceLocator> &AssetNode::GetReferences()
{
  return m_references;
}

const csResourceLocator AssetNode::WithNewName(const std::string &newName) const
{
  if (m_parent)
  {
    return m_parent->GetResourceLocator().WithFileSuffix(newName);
  }
  return csResourceLocator(csResourceFile(newName));

}


csResourceLocator AssetNode::GetNewResourceLocator()
{
  if (m_parent)
  {
    return m_parent->GetResourceLocator().WithFileSuffix(m_name);
  }
  return csResourceLocator(csResourceFile(m_name));
}

void AssetNode::DebugNode() const
{
  printf("AssetNode: %s\n", GetResourceLocator().Encode().c_str());
}

}
