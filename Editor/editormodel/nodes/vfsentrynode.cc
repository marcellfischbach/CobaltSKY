
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

bool VFSEntryNode::IsVFSEntryNode() const
{
  return true;
}

VFSEntryNode *VFSEntryNode::AsVFSEntryNode()
{
  return this;
}

const VFSEntryNode *VFSEntryNode::AsVFSEntryNode() const
{
  return this;
}

void VFSEntryNode::SetEntry(const cs::VFS::Entry *entry)
{
  m_entry = entry;
  UpdateResourceLocator();
}

const cs::VFS::Entry *VFSEntryNode::GetEntry() const
{
  return m_entry;
}

int VFSEntryNode::GetPriority() const
{
  return m_entry ? m_entry->GetPriority() : -1;
}


const cs::ResourceLocator VFSEntryNode::WithNewName(const std::string &newName) const
{
  if (m_entry)
  {
    return cs::ResourceLocator(
      cs::ResourceEntry(m_entry->GetName()),
      cs::ResourceFile("")
    );
  }

  return cs::ResourceLocator(
    cs::ResourceFile("")
  );

}

cs::ResourceLocator VFSEntryNode::GetNewResourceLocator()
{
  if (m_entry)
  {
    return cs::ResourceLocator(
      cs::ResourceEntry(m_entry->GetName()),
      cs::ResourceFile("")
    );
  }

  return cs::ResourceLocator(
      cs::ResourceFile("")
    );
  
}

void VFSEntryNode::DebugNode() const
{
  printf("VFSEntry: %s\n", m_entry->GetName().c_str());
}

}

