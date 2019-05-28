

#include <assetmanager/list/assetmanagerlistlistnode.hh>

#include <editormodel/nodes/assetnode.hh>
#include <editormodel/nodes/node.hh>
#include <editormodel/nodes/vfsentrynode.hh>

#include <algorithm>

namespace cs::editor::assetmanager
{


ListListNode::ListListNode(model::Node *node)
  : m_node(node)
  , m_parent(nullptr)
{

}

ListListNode::~ListListNode()
{
  for (auto child : m_children)
  {
    delete child;
  }
}

QString ListListNode::GetName() const
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


QString ListListNode::GetType() const
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

bool ListListNode::HasParent(const ListListNode *node) const
{
  const ListListNode *parent = this;
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

int ListListNode::GetIndexOfChild(const ListListNode *child, int ifOrfan) const
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

int ListListNode::GetIndexWithinParent(int ifOrfan) const
{
  return m_parent ? m_parent->GetIndexOfChild(this, ifOrfan) : ifOrfan;
}

size_t ListListNode::GetNumberOfChildren() const
{
  return m_children.size();
}

const ListListNode *ListListNode::GetChild(size_t idx) const
{
  return idx < m_children.size() ? m_children[idx] : nullptr;
}

bool ListListNode::less(ListListNode *node0, ListListNode *node1)
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

void ListListNode::Sort()
{
  std::sort(m_children.begin(), m_children.end(), ListListNode::less);
}

}