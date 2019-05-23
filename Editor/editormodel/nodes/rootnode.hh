
#pragma once

#include <editormodel/editormodelexport.hh>
#include <editormodel/nodes/node.hh>

namespace cs::editor::model
{

class EDITOR_MODEL_API RootNode : public Node
{
  friend class Model;
public:
  virtual ~RootNode();

  virtual bool IsRootNode() const override;
  virtual RootNode *AsRootNode() override;
  virtual const RootNode *AsRootNode() const override;

  virtual const cs::ResourceLocator WithNewName(const std::string &newName) const;

protected:
  cs::ResourceLocator GetNewResourceLocator();

  virtual void DebugNode() const;

private:
  RootNode(Model *model);

};

}
