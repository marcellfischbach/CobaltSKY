
#include <editormodel/nodes/rootnode.hh>

namespace cs::editor::model
{

RootNode::RootNode(Model *model)
  : Node(model, eT_Root)
{

}

RootNode::~RootNode()
{

}

bool RootNode::IsRootNode() const
{
  return true;
}

RootNode *RootNode::AsRootNode()
{
  return this;
}


const RootNode *RootNode::AsRootNode() const
{
  return this;
}



const cs::ResourceLocator RootNode::WithNewName(const std::string &newName) const
{
  // root node cannot have a valid resource locator
  return cs::ResourceLocator(cs::ResourceFile(""));

}

cs::ResourceLocator RootNode::GetNewResourceLocator()
{
  // root node cannot have a valid resource locator
  return cs::ResourceLocator(
    cs::ResourceFile("")
  );
}

void RootNode::DebugNode() const
{
  printf("Root\n");
}

}
