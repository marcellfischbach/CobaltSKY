
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

const csResourceLocator RootNode::WithNewName(const std::string &newName) const
{
  // root node cannot have a valid resource locator
  return csResourceLocator(csResourceFile(""));

}

csResourceLocator RootNode::GetNewResourceLocator()
{
  // root node cannot have a valid resource locator
  return csResourceLocator(
    csResourceFile("")
  );
}

void RootNode::DebugNode() const
{
  printf("Root\n");
}

}
