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
{

}

Model::~Model()
{

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
    RegisterAddCommit(child, toParent, tx);

  }
  tx.OnRollback([this, child, toParent]() { AddRollback(child, toParent);  });
}


void Model::RegisterAddCommit(Node *node, Node *toParent, Transaction &tx)
{
  tx.OnCommit([this, node, toParent]() { AddCommit(node, toParent); });

  for (auto child : node->GetChildren())
  {
    RegisterAddCommit(child, node, tx);
  }
}

void Model::AddCommit(Node *child, Node *toParent)
{
  m_onTreeStructNodeAdded.Call(child, toParent);
  SyncNodeToCache(child);

}


void Model::AddRollback(Node *child, Node *toParent)
{
  child->m_parent = nullptr;
  auto it = std::find(toParent->m_children.begin(), toParent->m_children.end(), child);
  toParent->m_children.erase(it);
}

void Model::Delete(Node *node, Transaction &tx)
{

}

void Model::Move(Node *child, Node *toNewParent, Transaction &tx)
{
}


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

  RegisterRenameCommit(node, tx);

  std::string oldName = node->GetName();
  node->SetName(newName);
  tx.OnRollback([this, node, oldName]() { RenameRollback(node, oldName); });
  SecureFS().Rename(oldPath, newPath, tx);
}

void Model::RegisterRenameCommit(Node *node, Transaction &tx)
{
  csResourceLocator oldLocator = node->GetResourceLocator();
  tx.OnCommit([this, node, oldLocator]() {RenameCommit(node, oldLocator); });
  for (auto child : node->GetChildren())
  {
    RegisterRenameCommit(child, tx);
  }
}

void Model::RenameCommit(Node *node, const csResourceLocator &oldLocator)
{
  SyncNodeToCache(node);
}


void Model::RenameRollback(Node *node, const std::string &oldName)
{
  node->SetName(oldName);
}


csResourceLocator Model::FindCurrentName(const Node *node)
{
  for (auto entry : m_namedNodes)
  {
    if (entry.second == node)
    {
      return entry.first;
    }
  }

  return csResourceLocator();
}


void Model::SyncNodeToCache(Node *node)
{
  csResourceLocator newLocator = node->GetResourceLocator();
  csResourceLocator oldLocator = FindCurrentName(node);
  m_namedNodes[newLocator] = node;
  if (oldLocator.IsValid())
  {
    // there is already an entry for thad node
    if (oldLocator == newLocator)
    {
      // the old name is the same as the new name so nothing could have changed
      return;
    }

    // remove the old entry because we have already added the new entry 
    auto it = m_namedNodes.find(oldLocator);
    m_namedNodes.erase(it);
    m_onNamedNodeNameChanged.Call(node, oldLocator, newLocator);
  }
  else
  {
    m_onNamedNodeNameAdded.Call(node, newLocator);
  }

  csResourceLocator anonNewLocator = newLocator.AsAnonymous();
  csResourceLocator anonOldLocator = oldLocator.AsAnonymous();

  bool wasMasterNode = IsMasterNode(node);

  if (anonNewLocator != anonOldLocator)
  {

    // handle renaming of the resource

    auto newSetIt = m_anonNodes.find(anonNewLocator);
    auto oldSetIt = m_anonNodes.find(anonOldLocator);


    if (oldSetIt != m_anonNodes.end())
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
        m_anonNodes.erase(oldSetIt);
      }
    }

    bool isANewEntry = newSetIt == m_anonNodes.end();


    // this node was previously stored by another name... 
    if (wasMasterNode)
    {
      // this was previously the master node, so we have a rename not only a remove
      m_onResourceNameRenamed.Call(anonOldLocator, anonNewLocator);
    }
    else if (isANewEntry)
    {
      m_onResourceNameAdded.Call(anonNewLocator);
    }

    m_anonNodes[anonNewLocator].insert(node);


    if (anonOldLocator.IsValid())
    {
      UpdateCurrentMasterEntry(anonOldLocator);
    }
    UpdateCurrentMasterEntry(anonNewLocator);
  }


}

bool Model::IsMasterNode(Node *node)
{
  for (auto e : m_anonMasterNode)
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

  auto it = m_anonNodes.find(locator.AsAnonymous());
  if (it != m_anonNodes.end())
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
    auto it = m_anonMasterNode.find(locator);
    if (it != m_anonMasterNode.end())
    {
      m_anonMasterNode.erase(it);
    }
  }
  else
  {
    m_anonMasterNode[locator] = node;
  }
}


const Node *Model::FindNode(const csResourceLocator &locator) const
{
  if (locator.IsAnonymous())
  {
    auto it = m_anonNodes.find(locator);
    if (it == m_anonNodes.end())
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
    auto it = m_namedNodes.find(locator);
    if (it == m_namedNodes.end())
    {
      return nullptr;
    }
    return it->second;
  }
}


const Node *Model::FindNode(const csResourceLocator &locator, MinPriority &minPriority) const
{
  csResourceLocator anonLocator = locator.AsAnonymous();
  auto it = m_anonNodes.find(anonLocator);
  if (it == m_anonNodes.end())
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
  auto it = m_anonNodes.find(anonLocator);
  if (it == m_anonNodes.end())
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



}

