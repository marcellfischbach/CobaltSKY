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

bool FolderNode::IsFolderNode() const
{
  return true;
}

FolderNode *FolderNode::AsFolderNode()
{
  return this;
}


const FolderNode *FolderNode::AsFolderNode() const
{
  return this;
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



const cs::ResourceLocator FolderNode::WithNewName(const std::string &newName) const
{
  if (m_parent)
  {
    return m_parent->GetResourceLocator().WithFileSuffix(newName + "/");
  }
  return cs::ResourceLocator(cs::ResourceFile(newName + "/"));

}


cs::ResourceLocator FolderNode::GetNewResourceLocator()
{
  if (m_parent)
  {
    return m_parent->GetResourceLocator().WithFileSuffix(m_name + "/");
  }
  return cs::ResourceLocator(cs::ResourceFile(m_name + "/"));
}

void FolderNode::DebugNode() const
{
  printf("FolderNode: %s\n", GetResourceLocator().Encode().c_str());
}

}
