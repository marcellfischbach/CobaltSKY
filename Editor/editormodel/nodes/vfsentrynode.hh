
#pragma once

#include <editormodel/editormodelexport.hh>
#include <editormodel/nodes/foldernode.hh>
#include <cobalt/core/csvfs.hh>

namespace cs::editor::model
{


class EDITOR_MODEL_API VFSEntryNode : public FolderNode
{
  friend class Model;
public:
  virtual ~VFSEntryNode();

public:

  virtual bool IsVFSEntryNode() const override;
  virtual VFSEntryNode *AsVFSEntryNode() override;
  virtual const VFSEntryNode *AsVFSEntryNode() const override;

  void SetEntry(const cs::VFS::Entry *entry);
  const cs::VFS::Entry *GetEntry() const;

  virtual const cs::ResourceLocator WithNewName(const std::string &newName) const;

  virtual int GetPriority() const override;
  
protected:
  cs::ResourceLocator GetNewResourceLocator();

  virtual void DebugNode() const;

private:
  VFSEntryNode(Model *model);

  const cs::VFS::Entry *m_entry;
};

}
