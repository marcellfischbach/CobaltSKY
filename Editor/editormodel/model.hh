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

  Transaction CreateTransaction();


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

  cs::Signal<Node*, Node*> & OnTreeStructNodeAdded() { return m_onTreeStructNodeAdded; }
  cs::Signal<Node*, Node*, Node*> & OnTreeStructNodeMoved() { return m_onTreeStructNodeMoved; }
  cs::Signal<Node*> & OnTreeStructNodeChanged() { return m_onTreeStructNodeChanged; }
  cs::Signal<Node*> & OnTreeStructNodeRemoved() { return m_onTreeStructNodeRemoved; }

  cs::Signal<Node*, csResourceLocator> &OnNamedNodeAdded() { return m_onNamedNodeAdded; }
  cs::Signal<Node*, csResourceLocator, csResourceLocator> &OnNamedNodeRenamed() { return m_onNamedNodeRenamed;  };
  cs::Signal<Node*, csResourceLocator> &OnNamedNodeRemoved() { return m_onNamedNodeRemoved; }


  cs::Signal<csResourceLocator> &OnResourceNameAdded() { return m_onResourceNameAdded;  }
  cs::Signal<csResourceLocator> &OnResourceNameChanged() { return m_onResourceNameChanged; }
  cs::Signal<csResourceLocator, csResourceLocator> &OnResourceNameRenamed() { return m_onResourceNameRenamed;  }
  cs::Signal<csResourceLocator> &OnResourceNameRemoved() { return m_onResourceNameRemoved;  }
  /**
   * @}
   */

  void Debug() const;

private:
  void AddRecursive(Node *child, Node *toParent, Transaction &tx);
  void AddRollback(Node *child, Node *toParent);

  void RenameRollback(Node* child, const std::string &oldName);

  csResourceLocator FindCurrentName(const Node *node);

  bool IsMasterNode(Node *node);
  void UpdateCurrentMasterEntry(const csResourceLocator &anonLocator);
  void SetCurrentMasterEntry(const csResourceLocator &anonLocator, Node *node);

  void TxCommit();
  void TxRollback();

  void SyncNodeCacheRecursive(Node *node, Transaction &tx);
  void SyncNodeCache(Node *node, Transaction &tx);
  void SyncNamedNode(Node *node, const csResourceLocator &oldLocator, const csResourceLocator &newLocator, Transaction &tx);
  void SyncAnonNode(Node *node, const csResourceLocator &oldLocator, const csResourceLocator &newLocator, Transaction &tx);


  RootNode * m_root;

  cs::Signal<Node*, Node*> m_onTreeStructNodeAdded;
  cs::Signal<Node*, Node*, Node*> m_onTreeStructNodeMoved;
  cs::Signal<Node*> m_onTreeStructNodeChanged;
  cs::Signal<Node*> m_onTreeStructNodeRemoved;


  cs::Signal<Node*, csResourceLocator> m_onNamedNodeAdded;
  cs::Signal<Node*, csResourceLocator, csResourceLocator> m_onNamedNodeRenamed;
  cs::Signal<Node*, csResourceLocator> m_onNamedNodeRemoved;

  cs::Signal<csResourceLocator> m_onResourceNameAdded;
  cs::Signal<csResourceLocator> m_onResourceNameChanged;
  cs::Signal<csResourceLocator, csResourceLocator> m_onResourceNameRenamed;
  cs::Signal<csResourceLocator> m_onResourceNameRemoved;

  struct Cache
  {

    std::map<csResourceLocator, Node*> NamedNodes;
    std::map<csResourceLocator, std::set<Node*>> AnonNodes;
    std::map<csResourceLocator, Node*> AnonMasterNode;

    std::map<Node*, std::set<csResourceLocator>> References;
    std::map<csResourceLocator, std::set<Node*>> ReferencedBy;
  };

  Cache m_cache;

  Cache *m_txCache;

  void Copy(const Cache &src, Cache &dst) const;
};

}
