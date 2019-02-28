#include <editormodel/nodes/foldernode.hh>

namespace cs::editor::model
{


FolderNode::FolderNode(Model *model, Type type)
  : Node(model, type)
{

}

FolderNode::~FolderNode()
{

}

void FolderNode::SetName(const std::string &name)
{
  m_name = name;
  UpdateResourceLocator();
}

const std::string &FolderNode::GetName() const
{
  return m_name;
}



const csResourceLocator FolderNode::WithNewName(const std::string &newName) const
{
  if (m_parent)
  {
    return m_parent->GetResourceLocator().WithFileSuffix(newName + "/");
  }
  return csResourceLocator(csResourceFile(newName + "/"));

}


csResourceLocator FolderNode::GetNewResourceLocator()
{
  if (m_parent)
  {
    return m_parent->GetResourceLocator().WithFileSuffix(m_name + "/");
  }
  return csResourceLocator(csResourceFile(m_name + "/"));
}

void FolderNode::DebugNode() const
{
  printf("FolderNode: %s\n", GetResourceLocator().Encode().c_str());
}

}
