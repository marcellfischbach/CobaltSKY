
#include <editormodel/nodes/vfsentrynode.hh>

namespace cs::editor::model
{

VFSEntryNode::VFSEntryNode(Model *model)
  : FolderNode (model, eT_VFSEntry)
  , m_entry(nullptr)
{

}

VFSEntryNode::~VFSEntryNode()
{

}

void VFSEntryNode::SetEntry(const csVFS::Entry *entry)
{
  m_entry = entry;
  UpdateResourceLocator();
}

const csVFS::Entry *VFSEntryNode::GetEntry() const
{
  return m_entry;
}

int VFSEntryNode::GetPriority() const
{
  return m_entry ? m_entry->GetPriority() : -1;
}


const csResourceLocator VFSEntryNode::WithNewName(const std::string &newName) const
{
  if (m_entry)
  {
    return csResourceLocator(
      csResourceEntry(m_entry->GetName()),
      csResourceFile("")
    );
  }

  return csResourceLocator(
    csResourceFile("")
  );

}

csResourceLocator VFSEntryNode::GetNewResourceLocator()
{
  if (m_entry)
  {
    return csResourceLocator(
      csResourceEntry(m_entry->GetName()),
      csResourceFile("")
    );
  }

  return csResourceLocator(
      csResourceFile("")
    );
  
}

void VFSEntryNode::DebugNode() const
{
  printf("VFSEntry: %s\n", m_entry->GetName().c_str());
}

}

