#pragma once

#include <editormodel/transaction.hh>
#include <editormodel/editormodelexport.hh>
#include <editormodel/modelexceptions.hh>
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
  void Move(Node *child, FolderNode *toNewParent, Transaction &tx);
  void Rename(Node *node, const std::string &newName, Transaction &tx);

  /**
   * @}
   */

  /**
   * @name Acquisition
   * @{
   */
  Node *FindNode(const cs::ResourceLocator &locator);
  const Node *FindNode(const cs::ResourceLocator &locator) const;

  Node *FindNode(const cs::ResourceLocator &locator, MinPriority &minPriority);
  const Node *FindNode(const cs::ResourceLocator &locator, MinPriority &minPriority) const;

  Node *FindNode(const cs::ResourceLocator &locator, MaxPriority &maxPriority);
  const Node *FindNode(const cs::ResourceLocator &locator, MaxPriority &maxPriority) const;

  const std::set<Node*> GetNodes(const cs::ResourceLocator &locator);
  const std::set<const Node*> GetNodes(const cs::ResourceLocator &locator) const;
  /**
   *@}
   */

   /**
    * @name Signals
    */

  /// Signal[Node *child, Node *toParent]
  cs::Signal<Node*, Node*> & OnTreeStructNodeAdded() { return m_onTreeStructNodeAdded; }
  /// Signal[Node *node, Node *oldParent, Node *newParent]
  cs::Signal<Node*, Node*, Node*> & OnTreeStructNodeMoved() { return m_onTreeStructNodeMoved; }
  /// Signal[Node *node]
  cs::Signal<Node*> & OnTreeStructNodeChanged() { return m_onTreeStructNodeChanged; }
  /// Signal[Node *child, Node *oldParent]
  cs::Signal<Node*, Node*> & OnTreeStructNodeRemoved() { return m_onTreeStructNodeRemoved; }

  cs::Signal<Node*, cs::ResourceLocator> &OnNamedNodeAdded() { return m_onNamedNodeAdded; }
  cs::Signal<Node*, cs::ResourceLocator, cs::ResourceLocator> &OnNamedNodeRenamed() { return m_onNamedNodeRenamed;  };
  cs::Signal<Node*, cs::ResourceLocator> &OnNamedNodeRemoved() { return m_onNamedNodeRemoved; }


  cs::Signal<cs::ResourceLocator> &OnResourceNameAdded() { return m_onResourceNameAdded;  }
  cs::Signal<cs::ResourceLocator> &OnResourceNameChanged() { return m_onResourceNameChanged; }
  cs::Signal<cs::ResourceLocator, cs::ResourceLocator> &OnResourceNameRenamed() { return m_onResourceNameRenamed;  }
  cs::Signal<cs::ResourceLocator> &OnResourceNameRemoved() { return m_onResourceNameRemoved;  }
  /**
   * @}
   */

  void Debug() const;

private:
  void AddRecursive(Node *child, Node *toParent, Transaction &tx);
  void AddRollback(Node *child, Node *toParent);

  void MoveAsset(AssetNode *assetNode, FolderNode *toParent, Transaction &tx);
  void MoveFolder(FolderNode *folderNode, FolderNode *toParent, Transaction &tx);
  void MergeFolder(FolderNode *folderNode, FolderNode *toParent, Transaction &tx);
  void MoveNodeToNewParent(Node *node, Node *oldParent, Node *toNewParent);


  cs::ResourceLocator FindCurrentName(const Node *node);

  bool IsMasterNode(Node *node);
  void UpdateCurrentMasterEntry(const cs::ResourceLocator &anonLocator);
  void SetCurrentMasterEntry(const cs::ResourceLocator &anonLocator, Node *node);

  void TxCommit();
  void TxRollback();

  void SyncNodeCacheRecursive(Node *node, Transaction &tx);
  void SyncNodeCache(Node *node, Transaction &tx);
  void SyncNamedNode(Node *node, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator, Transaction &tx);
  void SyncAnonNode(Node *node, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator, Transaction &tx);


  RootNode * m_root;

  /// Signal[Node *child, Node *toParent]
  cs::Signal<Node*, Node*>                                m_onTreeStructNodeAdded;
  /// Signal[Node *node, Node *oldParent, Node *newParent]
  cs::Signal<Node*, Node*, Node*>                         m_onTreeStructNodeMoved;
  /// Signal[Node *node]
  cs::Signal<Node*>                                       m_onTreeStructNodeChanged;
  /// Signal[Node *child, Node *oldParent]
  cs::Signal<Node*, Node*>                                m_onTreeStructNodeRemoved;


  cs::Signal<Node*, cs::ResourceLocator>                    m_onNamedNodeAdded;
  cs::Signal<Node*, cs::ResourceLocator, cs::ResourceLocator> m_onNamedNodeRenamed;
  cs::Signal<Node*, cs::ResourceLocator>                    m_onNamedNodeRemoved;

  cs::Signal<cs::ResourceLocator>                           m_onResourceNameAdded;
  cs::Signal<cs::ResourceLocator>                           m_onResourceNameChanged;
  cs::Signal<cs::ResourceLocator, cs::ResourceLocator>        m_onResourceNameRenamed;
  cs::Signal<cs::ResourceLocator>                           m_onResourceNameRemoved;

  struct Cache
  {

    std::map<cs::ResourceLocator, Node*> NamedNodes;
    std::map<cs::ResourceLocator, std::set<Node*>> AnonNodes;
    std::map<cs::ResourceLocator, Node*> AnonMasterNode;

    std::map<Node*, std::set<cs::ResourceLocator>> References;
    std::map<cs::ResourceLocator, std::set<Node*>> ReferencedBy;
  };

  Cache m_cache;

  Cache *m_txCache;

  void Copy(const Cache &src, Cache &dst) const;
};

}
