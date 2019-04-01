

#include <assetmanager/list/assetmanagerlisttreenode.hh>

#include <editormodel/nodes/assetnode.hh>
#include <editormodel/nodes/node.hh>
#include <editormodel/nodes/vfsentrynode.hh>

#include <algorithm>

namespace cs::editor::assetmanager
{


ListTreeNode::ListTreeNode(model::Node *node)
  : m_node(node)
  , m_parent(nullptr)
{

}

ListTreeNode::~ListTreeNode()
{
  for (auto child : m_children)
  {
    delete child;
  }
}

QString ListTreeNode::GetName() const
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
      return QString(m_node->GetName().c_str());
    case model::Node::eT_Asset:
      return QString("%1 (%2)").arg(m_node->GetName().c_str(), m_node->AsAssetNode()->GetAssetTypeName().c_str());
    }
  }
  return "";
}


QString ListTreeNode::GetType() const
{
  if (m_node)
  {
    if (m_node->IsAssetNode())
    {

      return QString(m_node->AsAssetNode()->GetAssetTypeName().c_str());
    }
  }
  return "";
}

bool ListTreeNode::HasParent(const ListTreeNode *node) const
{
  const ListTreeNode *parent = this;
  while (parent)
  {
    if (parent == node)
    {
      return true;
    }
    parent = parent->m_parent;
  }
  return false;
}

int ListTreeNode::GetIndexOfChild(const ListTreeNode *child, int ifOrfan) const
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

int ListTreeNode::GetIndexWithinParent(int ifOrfan) const
{
  return m_parent ? m_parent->GetIndexOfChild(this, ifOrfan) : ifOrfan;
}

size_t ListTreeNode::GetNumberOfChildren() const
{
  return m_children.size();
}

const ListTreeNode *ListTreeNode::GetChild(size_t idx) const
{
  return idx < m_children.size() ? m_children[idx] : nullptr;
}

bool ListTreeNode::less(ListTreeNode *node0, ListTreeNode *node1)
{
  model::Node *n0 = node0->m_node;
  model::Node *n1 = node1->m_node;

  if (n0->GetType() < n1->GetType())
  {
    return true;
  }
  else if (n1->GetType() < n0->GetType())
  {
    return false;
  }

  if (n0->IsVFSEntryNode() && n1->IsVFSEntryNode())
  {
    if (n0->GetPriority() != n1->GetPriority())
    {
      return n0->GetPriority() < n1->GetPriority();
    }
  }

  return QString::compare(node0->GetName(), node1->GetName(), Qt::CaseInsensitive) < 0;
}

void ListTreeNode::Sort()
{
  std::sort(m_children.begin(), m_children.end(), ListTreeNode::less);
}

}