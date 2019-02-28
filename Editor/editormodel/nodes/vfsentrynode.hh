
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
  void SetEntry(const csVFS::Entry *entry);
  const csVFS::Entry *GetEntry() const;

  virtual const csResourceLocator WithNewName(const std::string &newName) const;

  virtual int GetPriority() const override;
  
protected:
  csResourceLocator GetNewResourceLocator();

  virtual void DebugNode() const;

private:
  VFSEntryNode(Model *model);

  const csVFS::Entry *m_entry;
};

}
