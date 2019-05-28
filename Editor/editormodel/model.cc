#include <editormodel/model.hh>
#include <editormodel/nodescanner.hh>
#include <editormodel/transaction.hh>
#include <editormodel/nodes/assetnode.hh>
#include <editormodel/nodes/foldernode.hh>
#include <editormodel/nodes/rootnode.hh>
#include <editormodel/nodes/vfsentrynode.hh>
#include <editormodel/fs/secfs.hh>
#include <iostream>

namespace cs::editor::model
{

Model::Model()
  : m_root(new RootNode(this))
  , m_txCache(nullptr)
{

}

Model::~Model()
{

}

Transaction Model::CreateTransaction()
{
  if (m_txCache)
  {
    // there is already an active transaction so return an invalid one
    return Transaction(Transaction::eS_Invalid);
  }

  m_txCache = new Cache();
  Copy(m_cache, *m_txCache);


  Transaction tx;
  tx.OnCommit([this]() { TxCommit(); });
  tx.OnRollback([this]() { TxRollback(); });
  return tx;
}

RootNode *Model::GetRoot()
{
  return m_root;
}

const RootNode *Model::GetRoot() const
{
  return m_root;
}

AssetNode *Model::CreateAssetNode()
{
  return new AssetNode(this);
}

FolderNode *Model::CreateFolderNode()
{
  return new FolderNode(this);
}

VFSEntryNode *Model::CreateVFSEntryNode()
{
  return new VFSEntryNode(this);
}


/* ******************************************************************
 * ******************************************************************
 *
 *      Handling add
 *
 * ******************************************************************
 * ******************************************************************/


void Model::Add(Node *child, Node *toParent, Transaction &tx)
{
  if (!child || !toParent)
  {
    return;
  }

  auto it = std::find(toParent->m_children.begin(), toParent->m_children.end(), child);
  if (it != toParent->m_children.end() || child->m_parent)
  {
    // child already part of the parent or child already has a parent
    return;
  }

  if (child->IsAssetNode())
  {
    AssetNode *assetNode = child->AsAssetNode();
    NodeScanner ns;
    ns.Scan(assetNode);
    assetNode->SetAssetTypeName(ns.GetTypeName());
    assetNode->GetReferences() = ns.GetReferenceLocators();

  }

  toParent->m_children.push_back(child);
  child->m_parent = toParent;
  child->UpdateResourceLocator();

  if (toParent->IsAttached())
  {
    AddRecursive(child, toParent, tx);
  }
  tx.OnRollback([this, child, toParent]() { AddRollback(child, toParent);  });
}


void Model::AddRecursive(Node *node, Node *toParent, Transaction &tx)
{
  SyncNodeCache(node, tx);

  tx.OnCommit([this, node, toParent]() { m_onTreeStructNodeAdded.Call(node, toParent); });

  for (auto child : node->GetChildren())
  {
    AddRecursive(child, node, tx);
  }
}


void Model::AddRollback(Node *child, Node *toParent)
{
  child->m_parent = nullptr;
  auto it = std::find(toParent->m_children.begin(), toParent->m_children.end(), child);
  toParent->m_children.erase(it);
}


/* ******************************************************************
* ******************************************************************
*
*      Handling delete
*
* ******************************************************************
* ******************************************************************/


void Model::Delete(Node *node, Transaction &tx)
{
  if (!node)
  {
    return;
  }


  for (auto child : std::vector<Node*>(node->GetChildren()))
  {
    Delete(child, tx);
  }


  cs::ResourceLocator locator = node->GetResourceLocator();
  std::string locatorPath = cs::VFS::Get()->GetAbsolutePath(locator, cs::VFS::CheckExistence);
  if (locatorPath.empty())
  {
    return;
  }

  Node* parent = node->m_parent;
  if (!parent)
  {
    throw IllegalModelTreeStructException("Cannot delete element without a parent");
    return;
  }

  auto it = std::find(parent->m_children.begin(), parent->m_children.end(), node);
  if (it == parent->m_children.end())
  {
    throw IllegalModelTreeStructException("Node is not found with the children of its parent");
  }

  parent->m_children.erase(it);
  node->m_parent = nullptr;
  node->UpdateResourceLocator();

  SyncNodeCache(node, tx);


  tx.OnCommit([this, node, parent]() {
    m_onTreeStructNodeRemoved.Call(node, parent);

  });
  
  tx.OnRollback([this, node, parent]() {
    std::cout << "Rollback Delete: Reattach " << node->GetName() << " -> " << parent->GetResourceLocator().Encode() << std::endl;
    parent->m_children.push_back(node);
    node->m_parent = parent;
    node->UpdateResourceLocator();
  });

  std::filesystem::path path(locatorPath);
  SecureFS().Delete(path, tx);

}


/* ******************************************************************
* ******************************************************************
*
*      Handling move
*
* ******************************************************************
* ******************************************************************/


void Model::Move(Node *node, FolderNode *toNewParent, Transaction &tx)
{
  if (!node || !toNewParent)
  {
    return;
  }

  if (node->IsVFSEntryNode() || node->IsRootNode())
  {
    throw IllegalModelTreeStructException("Cannot move root or VFSEntry");
  }


  if (node->IsAssetNode())
  {
    MoveAsset(node->AsAssetNode(), toNewParent, tx);
  }
  else if (node->IsFolderNode())
  {
    MoveFolder(node->AsFolderNode(), toNewParent, tx);
  }
}

void Model::MoveAsset(AssetNode *assetNode, FolderNode *toNewParent, Transaction &tx)
{

  Node *oldParent = assetNode->m_parent;
  if (oldParent == toNewParent)
  {
    return;
  }

  cs::ResourceLocator assetLocator = assetNode->GetResourceLocator();
  std::string assetLocatorPath = cs::VFS::Get()->GetAbsolutePath(assetLocator, cs::VFS::CheckExistence);

  cs::ResourceLocator destinationLocator = toNewParent->GetResourceLocator();
  std::string destinationLocatorPath = cs::VFS::Get()->GetAbsolutePath(destinationLocator, cs::VFS::CheckExistence);

  if (assetLocatorPath.empty() || destinationLocatorPath.empty())
  {
    throw IllegalModelTreeStructException("Source or destination path is not set. Source: " + assetLocator.Encode() + "   Destination: " + destinationLocator.Encode());
  }


  MoveNodeToNewParent(assetNode, oldParent, toNewParent);


  tx.OnCommit([this, assetNode, oldParent, toNewParent]() {
    m_onTreeStructNodeMoved.Call(assetNode, oldParent, toNewParent);
  });

  tx.OnRollback([this, assetNode, oldParent, toNewParent]() {
    MoveNodeToNewParent(assetNode, toNewParent, oldParent);
  });

  SyncNodeCache(assetNode, tx);

  SecureFS().Move(std::filesystem::path(assetLocatorPath), std::filesystem::path(destinationLocatorPath), tx);




}

void Model::MoveNodeToNewParent(Node *node, Node *oldParent, Node *toNewParent)
{
  if (oldParent)
  {
    auto it = std::find(oldParent->m_children.begin(), oldParent->m_children.end(), node);
    if (it != oldParent->m_children.end())
    {
      oldParent->m_children.erase(it);
    }
  }

  toNewParent->m_children.push_back(node);
  node->m_parent = toNewParent;
  node->UpdateResourceLocator();

}


void Model::MoveFolder(FolderNode *folderNode, FolderNode *toNewParent, Transaction &tx)
{

  Node *oldParent = folderNode->m_parent;
  if (oldParent == toNewParent)
  {
    return;
  }

  //
  // check if there is already a folder with the same name within the new parent
  for (auto child : toNewParent->GetChildren())
  {
    if (child->IsFolderNode() && folderNode->GetName() == child->GetName())
    {
      MergeFolder(folderNode, child->AsFolderNode(), tx);
      return;
    }
  }


  //
  // if we get here there is no such folder within the destination folder
  // so we have a rename here actually
  cs::ResourceLocator oldLocator = folderNode->GetResourceLocator();
  cs::ResourceLocator newLocator = toNewParent->GetResourceLocator().WithFileSuffix(folderNode->GetName() + "/");

  std::filesystem::path oldPath(cs::VFS::Get()->GetAbsolutePath(oldLocator, cs::VFS::DontCheckExistence));
  std::filesystem::path newPath(cs::VFS::Get()->GetAbsolutePath(newLocator, cs::VFS::DontCheckExistence));

  MoveNodeToNewParent(folderNode, oldParent, toNewParent);

  SyncNodeCacheRecursive(folderNode, tx);

  SecureFS().Rename(oldPath, newPath, tx);

  tx.OnCommit([this, folderNode, oldParent, toNewParent]() {
    m_onTreeStructNodeMoved.Call(folderNode, oldParent, toNewParent);
  });

  tx.OnRollback([this, folderNode, oldParent, toNewParent]() {
    MoveNodeToNewParent(folderNode, toNewParent, oldParent);
  });
}

void Model::MergeFolder(FolderNode *folderNode, FolderNode *toNewParent, Transaction &tx)
{
  for (auto child : std::vector<Node*>(folderNode->GetChildren()))
  {
    Move(child, toNewParent, tx);
  }

  cs::ResourceLocator folderLocator = folderNode->GetResourceLocator();
  std::string folderLocatorPath = cs::VFS::Get()->GetAbsolutePath(folderLocator, cs::VFS::CheckExistence);

  SecureFS().Delete(std::filesystem::path(folderLocatorPath), tx);
}


/* ******************************************************************
* ******************************************************************
*
*      Handling rename
*
* ******************************************************************
* ******************************************************************/


void Model::Rename(Node *node, const std::string &newName, Transaction &tx)
{
  cs::ResourceLocator oldLocator = node->GetResourceLocator();
  cs::ResourceLocator newLocator = node->WithNewName(newName);
  if (oldLocator == newLocator)
  {
    return;
  }

  std::filesystem::path oldPath(cs::VFS::Get()->GetAbsolutePath(oldLocator, cs::VFS::DontCheckExistence));
  std::filesystem::path newPath(cs::VFS::Get()->GetAbsolutePath(newLocator, cs::VFS::DontCheckExistence));


  std::string oldName = node->GetName();
  node->SetName(newName);
  SyncNodeCacheRecursive(node, tx);

  SecureFS().Rename(oldPath, newPath, tx);
  tx.OnRollback([this, node, oldName]() { node->SetName(oldName); });

  tx.OnCommit([this, node]() { m_onTreeStructNodeChanged.Call(node); });
}




/* ******************************************************************
* ******************************************************************
*
*      Syncing
*
* ******************************************************************
* ******************************************************************/

void Model::SyncNodeCacheRecursive(Node *node, Transaction &tx)
{
  SyncNodeCache(node, tx);
  for (auto child : node->GetChildren())
  {
    SyncNodeCacheRecursive(child, tx);
  }
}

void Model::SyncNodeCache(Node *node, Transaction &tx)
{
  cs::ResourceLocator newLocator = node->IsAttached() ? node->GetResourceLocator() : cs::ResourceLocator::Invalid();
  cs::ResourceLocator oldLocator = FindCurrentName(node);

  SyncNamedNode(node, oldLocator, newLocator, tx);
  SyncAnonNode(node, oldLocator, newLocator, tx);
}

void Model::SyncNamedNode(Node *node, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator, Transaction &tx)
{
  if (oldLocator == newLocator)
  {
    // this is specially including if both locators are invalid

    // the old name is the same as the new name so nothing could have changed
    return;
  }


  if (newLocator.IsValid())
  {
    m_cache.NamedNodes[newLocator] = node;
  }

  if (oldLocator.IsValid())
  {
    // remove the old entry because we have already added the new entry 
    auto it = m_cache.NamedNodes.find(oldLocator);
    if (it != m_cache.NamedNodes.end())
    {
      m_cache.NamedNodes.erase(it);
    }
  }

  if (oldLocator.IsValid() && newLocator.IsValid())
  {
    // both locators are valid so this is a renaming
    tx.OnCommit([this, node, oldLocator, newLocator]() {m_onNamedNodeRenamed.Call(node, oldLocator, newLocator); });
  }
  else if (oldLocator.IsValid() && !newLocator.IsValid())
  {
    // only the old locator was valid so this was a removal
    tx.OnCommit([this, node, oldLocator]() { m_onNamedNodeRemoved.Call(node, oldLocator); });
  }
  else if (!oldLocator.IsValid() && newLocator.IsValid())
  {
    // only the new locator is valid so this was an add 
    tx.OnCommit([this, node, newLocator]() { m_onNamedNodeAdded.Call(node, newLocator); });
  }

}

void Model::SyncAnonNode(Node *node, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator, Transaction &tx)
{


  cs::ResourceLocator anonNewLocator = newLocator.AsAnonymous();
  cs::ResourceLocator anonOldLocator = oldLocator.AsAnonymous();
  if (anonNewLocator == anonOldLocator)
  {
    if (anonNewLocator.IsValid())
    {
      tx.OnCommit([this, anonNewLocator]() { m_onResourceNameChanged.Call(anonNewLocator); });
    }
    return;
  }




  bool wasMasterNode = IsMasterNode(node);
  bool oldNameRemoved = false;

  // remove the old entry from the anon set
  auto oldSetIt = m_cache.AnonNodes.find(anonOldLocator);
  if (oldSetIt != m_cache.AnonNodes.end())
  {
    // this node was stored with the old name, so remove it.
    auto oldSetEntryIt = oldSetIt->second.find(node);
    if (oldSetEntryIt != oldSetIt->second.end())
    {
      oldSetIt->second.erase(oldSetEntryIt);
    }

    if (oldSetIt->second.empty())
    {
      // this was the last entry with the old anon locator, so remove the entire set
      m_cache.AnonNodes.erase(oldSetIt);
      oldNameRemoved = true;

    }
    //
    // old locator has changed so update the old master entry
    UpdateCurrentMasterEntry(oldLocator);
  }


  if (!anonNewLocator.IsValid())
  {
    if (oldNameRemoved)
    {
      tx.OnCommit([this, anonOldLocator]() { m_onResourceNameRemoved.Call(anonOldLocator); });
      return;
    }
  }

  if (anonNewLocator.IsValid ())
  {
    // add the new name to the anon set
    auto newSetIt = m_cache.AnonNodes.find(anonNewLocator);
    bool isANewEntry = newSetIt == m_cache.AnonNodes.end();
    m_cache.AnonNodes[anonNewLocator].insert(node);

    //
    // new locator has changed so update the new master entry
    UpdateCurrentMasterEntry(newLocator);
    bool isMasterNode = IsMasterNode(node);



    if (wasMasterNode)
    {
      // this was a moving from an old master entry so we have a renaming from the old locator to the new locator
      // it doesn't matter in this case if the new anon entry is a master entry aswell
      tx.OnCommit([this, anonOldLocator, anonNewLocator]() { m_onResourceNameRenamed.Call(anonOldLocator, anonNewLocator); });
    }
    else if (isANewEntry)
    {
      // we astablished a new name in the anon locators
      tx.OnCommit([this, anonNewLocator]() { m_onResourceNameAdded.Call(anonNewLocator); });
    }
  }
}




/* ******************************************************************
* ******************************************************************
*
*      Other stuff
*
* ******************************************************************
* ******************************************************************/


cs::ResourceLocator Model::FindCurrentName(const Node *node)
{
  for (auto entry : m_cache.NamedNodes)
  {
    if (entry.second == node)
    {
      return entry.first;
    }
  }

  return cs::ResourceLocator();
}



bool Model::IsMasterNode(Node *node)
{
  for (auto e : m_cache.AnonMasterNode)
  {
    if (e.second == node)
    {
      return true;
    }
  }
  return false;
}

void Model::UpdateCurrentMasterEntry(const cs::ResourceLocator &locator)
{
  Node *node = nullptr;

  auto it = m_cache.AnonNodes.find(locator.AsAnonymous());
  if (it != m_cache.AnonNodes.end())
  {
    int lowestPrio = INT_MAX;
    for (auto n : it->second)
    {
      int nPrio = n->GetPriority();
      if (nPrio < lowestPrio)
      {
        lowestPrio = nPrio;
        node = n;
      }
    }
  }

  SetCurrentMasterEntry(locator.AsAnonymous(), node);
}


void Model::SetCurrentMasterEntry(const cs::ResourceLocator &locator, Node *node)
{
  if (node == nullptr)
  {
    auto it = m_cache.AnonMasterNode.find(locator);
    if (it != m_cache.AnonMasterNode.end())
    {
      m_cache.AnonMasterNode.erase(it);
    }
  }
  else
  {
    m_cache.AnonMasterNode[locator] = node;
  }
}


const Node *Model::FindNode(const cs::ResourceLocator &locator) const
{
  if (locator.IsAnonymous())
  {
    auto it = m_cache.AnonNodes.find(locator);
    if (it == m_cache.AnonNodes.end())
    {
      return nullptr;
    }

    int priority = INT_MIN;
    const Node *res = nullptr;
    for (const Node * node : it->second)
    {
      int nodePrio = node->GetPriority();
      if (nodePrio > priority)
      {
        priority = nodePrio;
        res = node;
      }
    }
    return res;
  }
  else
  {
    auto it = m_cache.NamedNodes.find(locator);
    if (it == m_cache.NamedNodes.end())
    {
      return nullptr;
    }
    return it->second;
  }
}


const Node *Model::FindNode(const cs::ResourceLocator &locator, MinPriority &minPriority) const
{
  cs::ResourceLocator anonLocator = locator.AsAnonymous();
  auto it = m_cache.AnonNodes.find(anonLocator);
  if (it == m_cache.AnonNodes.end())
  {
    return nullptr;
  }

  int priority = INT_MIN;
  const Node *res = nullptr;
  for (const Node * node : it->second)
  {
    int nodePrio = node->GetPriority();
    if (nodePrio > priority && nodePrio >= minPriority.GetPriority())
    {
      priority = nodePrio;
      res = node;
    }
  }
  return res;
}


const Node *Model::FindNode(const cs::ResourceLocator &locator, MaxPriority &maxPriority) const
{
  cs::ResourceLocator anonLocator = locator.AsAnonymous();
  auto it = m_cache.AnonNodes.find(anonLocator);
  if (it == m_cache.AnonNodes.end())
  {
    return nullptr;
  }

  int priority = INT_MIN;
  const Node *res = nullptr;
  for (const Node * node : it->second)
  {
    int nodePrio = node->GetPriority();
    if (nodePrio > priority && nodePrio <= maxPriority.GetPriority())
    {
      priority = nodePrio;
      res = node;
    }
  }
  return res;
}

Node *Model::FindNode(const cs::ResourceLocator &locator)
{
  return const_cast<Node*>(static_cast<const Model*>(this)->FindNode(locator));
}

Node *Model::FindNode(const cs::ResourceLocator &locator, MinPriority &minPriority)
{
  return const_cast<Node*>(static_cast<const Model*>(this)->FindNode(locator, minPriority));
}

Node *Model::FindNode(const cs::ResourceLocator &locator, MaxPriority &maxPriority)
{
  return const_cast<Node*>(static_cast<const Model*>(this)->FindNode(locator, maxPriority));
}


const std::set<Node*> Model::GetNodes(const cs::ResourceLocator &locator)
{
  auto it = m_cache.AnonNodes.find(locator.AsAnonymous());
  if (it == m_cache.AnonNodes.end())
  {
    return std::set<Node*>();
  }

  return it->second;
}

const std::set<const Node*> Model::GetNodes(const cs::ResourceLocator &locator) const
{
  auto it = m_cache.AnonNodes.find(locator.AsAnonymous());
  if (it == m_cache.AnonNodes.end())
  {
    return std::set<const Node*>();
  }

  std::set<const Node*> result;
  for (auto node : it->second)
  {
    result.insert(node);
  }
  return result;
}

void Model::Debug() const
{
  m_root->Debug(0);
}


void Model::TxCommit()
{
  if (m_txCache)
  {
    delete m_txCache;
    m_txCache = nullptr;
  }
}

void Model::TxRollback()
{
  if (m_txCache)
  {
    Copy(*m_txCache, m_cache);
    delete m_txCache;
    m_txCache = nullptr;
  }
}

void Model::Copy(const Cache &src, Cache &dst) const
{
  dst.AnonMasterNode = src.AnonMasterNode;
  dst.AnonNodes = src.AnonNodes;
  dst.NamedNodes = src.NamedNodes;
  dst.ReferencedBy = src.ReferencedBy;
  dst.References = src.References;
}

}

