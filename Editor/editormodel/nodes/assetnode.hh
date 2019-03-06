
#pragma once

#include <editormodel/editormodelexport.hh>
#include <editormodel/nodes/node.hh>
#include <set>

namespace cs::editor::model
{

class EDITOR_MODEL_API AssetNode : public Node
{
  friend class Model;
public:
  virtual ~AssetNode();

  virtual bool IsAssetNode() const override;
  virtual AssetNode *AsAssetNode() override;
  virtual const AssetNode *AsAssetNode() const override;

  virtual void SetName(const std::string &name) override;
  virtual const std::string &GetName() const override;

  virtual const csResourceLocator WithNewName(const std::string &newName) const;

  std::set<std::string> &GetReferences();


protected:
  AssetNode(Model *model);

  csResourceLocator GetNewResourceLocator();

  virtual void DebugNode() const;

private:
  std::string m_name;

  std::set<std::string> m_references;
};

}
