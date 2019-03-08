

#include <assetmanager/tree/assetmanagertreenode.hh>

#include <editormodel/nodes/node.hh>
#include <editormodel/nodes/vfsentrynode.hh>

namespace cs::editor::assetmanager
{

TreeNode::TreeNode(const model::Node *node)
  : m_node(node)
  , m_parent(nullptr)
{

}

QString TreeNode::GetName() const
{
  if (m_node)
  {
    switch (m_node->GetType())
    {
    case model::Node::eT_Root:
      return QString("Assets");
    case model::Node::eT_VFSEntry:
      {
        const model::VFSEntryNode *vfsEntry = m_node->AsVFSEntryNode();
        return QString("%1 (%2)").arg(vfsEntry->GetEntry()->GetName().c_str()).arg(vfsEntry->GetPriority());
      }
      
      break;
    case model::Node::eT_Folder:
    case model::Node::eT_Asset:
      return QString(m_node->GetName().c_str());
    }
  }
  return "";
}


QString TreeNode::GetType() const
{
  if (m_node)
  {
    if (m_node->IsAssetNode())
    {
      return QString("Asset");
    }
  }
  return "";
}

int TreeNode::GetIndexOfChild(const TreeNode *child, int ifOrfan) const
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    if (m_children[i] == child)
    {
      return static_cast<int>(i);
    }
  }
  return ifOrfan;
}

int TreeNode::GetIndexWithinParent(int ifOrfan) const
{
  return m_parent ? m_parent->GetIndexOfChild(this, ifOrfan) : ifOrfan;
}

size_t TreeNode::GetNumberOfChildren() const
{
  return m_children.size();
}

const TreeNode *TreeNode::GetChild(size_t idx) const
{
  return idx < m_children.size() ? m_children[idx] : nullptr;
}

}