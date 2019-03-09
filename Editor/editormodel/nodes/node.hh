
#pragma once

#include <editormodel/editormodelexport.hh>
#include <string>
#include <vector>
#include <cobalt/core/csresourcelocator.hh>

namespace cs::editor::model
{

class Model;
class AssetNode;
class FolderNode;
class RootNode;
class VFSEntryNode;
class EDITOR_MODEL_API Node
{
  friend class Model;

public:
  enum Type
  {
    eT_Root,
    eT_Folder,
    eT_VFSEntry,
    eT_Asset,
    eT_SIZE,
    eT_FORCE64 = 0xffffffffffffffff
  };
public:
  virtual ~Node();

  Type GetType() const;

  Model *GetModel();
  const Model *GetModel() const;

  bool IsAttached() const;

  virtual void SetName (const std::string &name);
  virtual const std::string &GetName() const;

  const csResourceLocator &GetResourceLocator() const;
  virtual const csResourceLocator WithNewName(const std::string &newName) const = 0;

  virtual bool IsAssetNode() const;
  virtual AssetNode *AsAssetNode();
  virtual const AssetNode *AsAssetNode() const;

  virtual bool IsFolderNode() const;
  virtual FolderNode *AsFolderNode();
  virtual const FolderNode *AsFolderNode() const;

  virtual bool IsRootNode() const;
  virtual RootNode *AsRootNode();
  virtual const RootNode *AsRootNode() const;

  virtual bool IsVFSEntryNode() const;
  virtual VFSEntryNode *AsVFSEntryNode();
  virtual const VFSEntryNode *AsVFSEntryNode() const;

  const std::vector<Node*> &GetChildren() const;

  virtual int GetPriority() const;

  void Debug(unsigned indent) const;


protected:
  Node(Model *model, Type type);

  void UpdateResourceLocator();
  virtual csResourceLocator GetNewResourceLocator() = 0;

  virtual void DebugNode() const = 0;

private:
  std::vector<Node*> &GetChildren();


protected:
  Type m_type;
  Model *m_model;

  csResourceLocator m_locator;

  Node *m_parent;
  std::vector<Node*> m_children;
};


}

