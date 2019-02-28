#pragma once

#include <editormodel/transaction.hh>
#include <editormodel/editormodelexport.hh>
#include <editormodel/nodes/assetnode.hh>
#include <editormodel/nodes/foldernode.hh>
#include <editormodel/nodes/rootnode.hh>
#include <editormodel/nodes/vfsentrynode.hh>

#include <cobalt/core/cssignalslot.hh>

#include <string>
#include <vector>
#include <map>



namespace cs::editor::model
{


class MinPriority
{
public:
  explicit MinPriority(int priority)
    : m_priority(priority)
  {

  }

  int GetPriority() const
  {
    return m_priority;
  }

private:
  int m_priority;
};


class MaxPriority
{
public:
  explicit MaxPriority(int priority)
    : m_priority(priority)
  {

  }

  int GetPriority() const
  {
    return m_priority;
  }

private:
  int m_priority;
};

class EDITOR_MODEL_API Model
{
public:
  Model();
  ~Model();

  AssetNode *CreateAssetNode();
  FolderNode *CreateFolderNode();
  VFSEntryNode *CreateVFSEntryNode();

  RootNode *GetRoot();
  const RootNode *GetRoot() const;


  /**
   * @name Public interface
   * @{
   */
  void Add(Node *child, Node *toParent, Transaction &tx);
  void Delete(Node *child, Transaction &tx);
  void Move(Node *child, Node *toNewParent, Transaction &tx);
  void Rename(Node *node, const std::string &newName, Transaction &tx);

  /**
   * @}
   */

  /**
   * @name Acquisition
   * @{
   */
  Node *FindNode(const csResourceLocator &locator);
  const Node *FindNode(const csResourceLocator &locator) const;

  Node *FindNode(const csResourceLocator &locator, MinPriority &minPriority);
  const Node *FindNode(const csResourceLocator &locator, MinPriority &minPriority) const;

  Node *FindNode(const csResourceLocator &locator, MaxPriority &maxPriority);
  const Node *FindNode(const csResourceLocator &locator, MaxPriority &maxPriority) const;
  /**
   *@}
   */

   /**
    * @name Signals
    */

  /**
   * \brief Emitted when a node is added to another node within the tree structure.
   */
  cs::Signal<Node*, Node*> & OnTreeStructNodeAdded() { return m_onTreeStructNodeAdded; }

  /**
  * \brief Emitted when a node is moved from one node to another within the tree structure.
  */
  cs::Signal<Node*, Node*, Node*> & OnTreeStructNodeMoved() { return m_onTreeStructNodeMoved; }

  /**
  * \brief Emitted when a node is moved from one node to another within the tree structure.
  */
  cs::Signal<Node*> & OnTreeStructNodeRemoved() { return m_onTreeStructNodeRemoved; }

  /**
   * \brief Emitted when a named node is added to the model
   */ 
  cs::Signal<Node*, csResourceLocator> &OnNamedNodeNameAdded() { return m_onNamedNodeNameAdded; }
  /**
   * \brief Emitted whan a named node is renamed
   */
  cs::Signal<Node*, csResourceLocator, csResourceLocator> &OnNamedNodeNameChanged() { return m_onNamedNodeNameChanged;  };

  cs::Signal<csResourceLocator> &OnResourceNameAdded() { return m_onResourceNameAdded;  }

  cs::Signal<csResourceLocator, csResourceLocator> &OnResourceNameRenamed() { return m_onResourceNameRenamed;  }

  cs::Signal<csResourceLocator> &OnResourceNameRemoved() { return m_onResourceNameRemoved;  }
  /**
   * @}
   */

  void Debug() const;

private:
  void RegisterAddCommit(Node *child, Node *toParent, Transaction &tx);
  void AddCommit(Node *child, Node *toParent);
  void AddRollback(Node *child, Node *toParent);

  void RegisterRenameCommit(Node *node, Transaction &tx);
  void RenameCommit(Node* child, const csResourceLocator &oldLocator);
  void RenameRollback(Node* child, const std::string &oldName);

  csResourceLocator FindCurrentName(const Node *node);

  void SyncNodeToCache(Node *node);
  bool IsMasterNode(Node *node);
  void UpdateCurrentMasterEntry(const csResourceLocator &anonLocator);
  void SetCurrentMasterEntry(const csResourceLocator &anonLocator, Node *node);

  RootNode * m_root;

  cs::Signal<Node*, Node*> m_onTreeStructNodeAdded;
  cs::Signal<Node*, Node*, Node*> m_onTreeStructNodeMoved;
  cs::Signal<Node*> m_onTreeStructNodeRemoved;


  cs::Signal<Node*> m_onNodeChanged;
  cs::Signal<Node*, csResourceLocator> m_onNamedNodeNameAdded;
  cs::Signal<Node*, csResourceLocator, csResourceLocator> m_onNamedNodeNameChanged;

  cs::Signal<csResourceLocator> m_onResourceNameAdded;
  cs::Signal<csResourceLocator, csResourceLocator> m_onResourceNameRenamed;
  cs::Signal<csResourceLocator> m_onResourceNameRemoved;

  std::map<csResourceLocator, Node*> m_namedNodes;
  std::map<csResourceLocator, std::set<Node*>> m_anonNodes;
  std::map<csResourceLocator, Node*> m_anonMasterNode;

  std::map<Node*, std::set<csResourceLocator>> m_references;
  std::map<csResourceLocator, std::set<Node*>> m_referencedBy;
};

}
