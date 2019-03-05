#include <editormodel/model.hh>
#include <editormodel/transaction.hh>
#include <editormodel/nodes/assetnode.hh>
#include <editormodel/nodes/foldernode.hh>
#include <editormodel/nodes/rootnode.hh>
#include <editormodel/nodes/vfsentrynode.hh>
#include <editormodel/fs/secfs.hh>

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

}


/* ******************************************************************
* ******************************************************************
*
*      Handling move
*
* ******************************************************************
* ******************************************************************/


void Model::Move(Node *child, Node *toNewParent, Transaction &tx)
{
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
  csResourceLocator oldLocator = node->GetResourceLocator();
  csResourceLocator newLocator = node->WithNewName(newName);
  if (oldLocator == newLocator)
  {
    return;
  }

  std::filesystem::path oldPath(csVFS::Get()->GetAbsolutePath(oldLocator, csVFS::DontCheckExistence));
  std::filesystem::path newPath(csVFS::Get()->GetAbsolutePath(newLocator, csVFS::DontCheckExistence));


  std::string oldName = node->GetName();
  node->SetName(newName);
  SyncNodeCacheRecursive(node, tx);

  SecureFS().Rename(oldPath, newPath, tx);
  tx.OnRollback([this, node, oldName]() { RenameRollback(node, oldName); });

  tx.OnCommit([this, node]() { m_onTreeStructNodeChanged.Call(node); });
}


void Model::RenameRollback(Node *node, const std::string &oldName)
{
  node->SetName(oldName);
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
  csResourceLocator newLocator = node->GetResourceLocator();
  csResourceLocator oldLocator = FindCurrentName(node);

  SyncNamedNode(node, oldLocator, newLocator, tx);
  SyncAnonNode(node, oldLocator, newLocator, tx);
}

void Model::SyncNamedNode(Node *node, const csResourceLocator &oldLocator, const csResourceLocator &newLocator, Transaction &tx)
{
  if (oldLocator == newLocator)
  {
    // is is specially including if both locators are invalid

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

void Model::SyncAnonNode(Node *node, const csResourceLocator &oldLocator, const csResourceLocator &newLocator, Transaction &tx)
{


  csResourceLocator anonNewLocator = newLocator.AsAnonymous();
  csResourceLocator anonOldLocator = oldLocator.AsAnonymous();

  if (anonNewLocator == anonOldLocator)
  {
    if (anonNewLocator.IsValid())
    {
      tx.OnCommit([this, anonNewLocator]() { m_onResourceNameChanged.Call(anonNewLocator); });
    }
    return;
  }




  bool wasMasterNode = IsMasterNode(node);

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
    }
    //
    // old locator has changed so update the old master entry
    UpdateCurrentMasterEntry(oldLocator);
  }



  
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




/* ******************************************************************
* ******************************************************************
*
*      Other stuff
*
* ******************************************************************
* ******************************************************************/


csResourceLocator Model::FindCurrentName(const Node *node)
{
  for (auto entry : m_cache.NamedNodes)
  {
    if (entry.second == node)
    {
      return entry.first;
    }
  }

  return csResourceLocator();
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

void Model::UpdateCurrentMasterEntry(const csResourceLocator &locator)
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


void Model::SetCurrentMasterEntry(const csResourceLocator &locator, Node *node)
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


const Node *Model::FindNode(const csResourceLocator &locator) const
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


const Node *Model::FindNode(const csResourceLocator &locator, MinPriority &minPriority) const
{
  csResourceLocator anonLocator = locator.AsAnonymous();
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


const Node *Model::FindNode(const csResourceLocator &locator, MaxPriority &maxPriority) const
{
  csResourceLocator anonLocator = locator.AsAnonymous();
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

Node *Model::FindNode(const csResourceLocator &locator)
{
  return const_cast<Node*>(static_cast<const Model*>(this)->FindNode(locator));
}

Node *Model::FindNode(const csResourceLocator &locator, MinPriority &minPriority)
{
  return const_cast<Node*>(static_cast<const Model*>(this)->FindNode(locator, minPriority));
}

Node *Model::FindNode(const csResourceLocator &locator, MaxPriority &maxPriority)
{
  return const_cast<Node*>(static_cast<const Model*>(this)->FindNode(locator, maxPriority));
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

