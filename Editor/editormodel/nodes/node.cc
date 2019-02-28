
#include <editormodel/nodes/node.hh>
#include <editormodel/illegaltypeexception.hh>

namespace cs::editor::model
{

Node::Node(Model *model, Type type)
  : m_type(type)
  , m_model(model)
  , m_locator("")
  , m_parent(nullptr)
{

}

Node::~Node()
{

}


void Node::SetName(const std::string &name)
{
  throw IllegalTypeException("SetName is not implement in this type");
}

const std::string &Node::GetName() const
{
  throw IllegalTypeException("GetName is not implement in this type");
}


Node::Type Node::GetType() const
{
  return m_type;
}

Model *Node::GetModel()
{
  return m_model;
}

const Model *Node::GetModel() const
{
  return m_model;
}

bool Node::IsAttached() const
{
  const Node *node = this;
  while (node)
  {
    if (node->m_type == eT_Root)
    {
      return true;
    }
    node = node->m_parent;
  }
  return false;
}

const csResourceLocator &Node::GetResourceLocator() const
{
  return m_locator;
}


AssetNode *Node::AsAssetNode()
{
  return nullptr;
}

const AssetNode *Node::AsAssetNode() const
{
  return nullptr;
}

FolderNode *Node::AsFolderNode()
{
  return nullptr;
}


const FolderNode *Node::AsFolderNode() const
{
  return nullptr;
}

RootNode *Node::AsRootNode()
{
  return nullptr;
}

const RootNode *Node::AsRootNode() const
{
  return nullptr;
}

VFSEntryNode *Node::AsVFSEntryNode()
{
  return nullptr;
}

const VFSEntryNode *Node::AsVFSEntryNode() const
{
  return nullptr;
}

const std::vector<Node*> &Node::GetChildren() const
{
  return m_children;
}


std::vector<Node*> &Node::GetChildren()
{
  return m_children;
}



int Node::GetPriority() const
{
  return m_parent ? m_parent->GetPriority() : -1;
}

void Node::UpdateResourceLocator()
{
  m_locator = GetNewResourceLocator();
  for (auto child : m_children)
  {
    child->UpdateResourceLocator();
  }
}

void Node::Debug(unsigned indent) const
{
  for (unsigned i = 0; i < indent; ++i)
  {
    printf("  ");
  }

  DebugNode();

  for (auto child : m_children)
  {
    child->Debug(indent + 1);
  }
}


}
