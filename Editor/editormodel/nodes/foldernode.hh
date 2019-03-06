
#pragma once

#include <editormodel/editormodelexport.hh>
#include <editormodel/nodes/node.hh>

namespace cs::editor::model
{


class EDITOR_MODEL_API FolderNode : public Node
{
  friend class Model;
public:
  virtual ~FolderNode();

  virtual bool IsFolderNode() const override;
  virtual FolderNode *AsFolderNode() override;
  virtual const FolderNode *AsFolderNode() const override;


  virtual void SetName(const std::string &name) override;
  virtual const std::string &GetName() const override;

  virtual const csResourceLocator WithNewName(const std::string &newName) const;


protected:
  FolderNode(Model *model, Type type = eT_Folder);

  csResourceLocator GetNewResourceLocator();

  virtual void DebugNode() const;

private:
  std::string m_name;
};

}
