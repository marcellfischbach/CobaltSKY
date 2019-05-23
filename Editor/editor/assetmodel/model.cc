
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/asset.hh>
#include <editor/assetmodel/exceptions.hh>
#include <editor/assetmodel/folder.hh>
#include <editor/assetmodel/modeltransaction.hh>
#include <editor/assetmodel/root.hh>
#include <editor/assetmodel/vfsentry.hh>
#include <editor/assetmodel/sync/assetmodifier.hh>
#include <editor/assetmodel/sync/securefs.hh>
#include <editor/eventbus.hh>
#include <editor/events/assetrenamedevent.hh>
#include <iostream>

namespace asset::model
{



Model::Model()
  : QObject()
{
  m_root = new Root(this);
}

Model::~Model()
{

}


Asset *Model::CreateAsset(const std::string &fileName)
{
  return new Asset(this, fileName);
}

Folder *Model::CreateFolder(const std::string &folderName)
{
  return new Folder(this, folderName);
}

VFSEntry *Model::CreateVFSEntry(const cs::VFS::Entry &entry)
{
  return new VFSEntry(this, entry);
}



bool Model::IsRootLocator(const cs::ResourceLocator &locator) const
{
  auto it = m_entries.find(locator.AsAnonymous());
  if (it == m_entries.end())
  {
    throw NoSuchLocatorException(locator.Encode());
  }

  auto ait = m_namedEntries.find(locator);
  if (ait == m_namedEntries.end())
  {
    throw NoSuchLocatorException(locator.Encode());
  }

  int prio = ait->second->GetVFSEntryPriority();
  int lowestPrio = INT_MAX;
  for (asset::model::Entry* entry : it->second)
  {
    try
    {
      int assetPrio = entry->GetVFSEntryPriority();
      lowestPrio = std::min<int>(assetPrio, lowestPrio);
    }
    catch (NotAttachedException &)
    {

    }
  }
  return prio == lowestPrio;
}

bool Model::IsFinalLocator(const cs::ResourceLocator &locator) const
{
  auto it = m_entries.find(locator.AsAnonymous());
  if (it == m_entries.end())
  {
    throw NoSuchLocatorException(locator.Encode());
  }

  auto ait = m_namedEntries.find(locator);
  if (ait == m_namedEntries.end())
  {
    throw NoSuchLocatorException(locator.Encode());
  }

  int prio = ait->second->GetVFSEntryPriority();
  int highestPrio = 0;
  for (asset::model::Entry* entry : it->second)
  {
    try
    {
      int assetPrio = entry->GetVFSEntryPriority();
      highestPrio = std::max<int>(assetPrio, highestPrio);
    }
    catch (NotAttachedException &)
    {

    }
  }
  return prio == highestPrio;
}


Entry *Model::GetFinalEntry(Entry *entry)
{
  return const_cast<Entry*>(
        static_cast<const Model*>(this)->GetFinalEntry(entry)
        );
}

const Entry *Model::GetFinalEntry(Entry *entry) const
{
  if (!entry)
  {
    return nullptr;
  }

  return GetFinalEntry(entry->GetResourceLocator());
}

Entry *Model::GetFinalEntry(const cs::ResourceLocator &locator)
{
  return const_cast<Entry*>(
        static_cast<const Model*>(this)->GetFinalEntry(locator)
        );
}


const Entry *Model::GetFinalEntry(const cs::ResourceLocator &locator) const
{
  auto entries = m_entries.find(locator.AsAnonymous());
  int prio = 0;
  Entry *finalEntry = nullptr;
  for (auto entry : entries->second)
  {
    try
    {
      if (!finalEntry || entry->GetVFSEntryPriority() > prio)
      {
        prio = entry->GetVFSEntryPriority();
        finalEntry = entry;
      }
    }
    catch (NotAttachedException&)
    {

    }
  }
  return finalEntry;
}

Entry *Model::GetRootEntry(Entry *entry)
{
  return const_cast<Entry*>(
        static_cast<const Model*>(this)->GetRootEntry(entry)
        );
}

const Entry *Model::GetRootEntry(Entry *entry) const
{
  if (!entry)
  {
    return nullptr;
  }

  return GetRootEntry(entry->GetResourceLocator());
}

Entry *Model::GetRootEntry(const cs::ResourceLocator &locator)
{
  return const_cast<Entry*>(
        static_cast<const Model*>(this)->GetRootEntry(locator)
        );
}


const Entry *Model::GetRootEntry(const cs::ResourceLocator &locator) const
{
  auto entries = m_entries.find(locator.AsAnonymous());
  int prio = INT_MAX;
  Entry *rootEntry = nullptr;
  for (auto entry : entries->second)
  {
    try
    {
      if (!rootEntry || entry->GetVFSEntryPriority() < prio)
      {
        prio = entry->GetVFSEntryPriority();
        rootEntry = entry;
      }
    }
    catch(NotAttachedException&)
    {

    }
  }
  return rootEntry;
}

const Entry *Model::GetEntry(const cs::ResourceLocator &entry) const
{

  auto it = m_namedEntries.find(entry);
  if (it == m_namedEntries.end())
  {
    return nullptr;
  }
  return it->second;
}


Entry *Model::GetEntry(const cs::ResourceLocator &entry)
{
  return const_cast<Entry*>(
        static_cast<const Model*>(this)->GetEntry(entry)
        );
}

const std::set<Entry*> &Model::GetAllEntriesFor(const cs::ResourceLocator &locator) const
{
  auto it = m_entries.find(locator.AsAnonymous());
  if (it == m_entries.end())
  {
    throw NoSuchLocatorException(locator.AsAnonymous());
  }
  return it->second;
}

std::set<Entry*> Model::GetReferencing(const cs::ResourceLocator &locator) const
{
  cs::ResourceLocator anonLocator = locator.AsAnonymous();
  std::set<Entry*> entriesReferencingLocator;
  for (auto it = m_references.begin(); it != m_references.end(); ++it)
  {
    if (it->second.find(anonLocator) != it->second.end())
    {
      entriesReferencingLocator.insert(it->first);
    }
  }
  return entriesReferencingLocator;
}

int Model::GetRootPriority(const cs::ResourceLocator &locator) const
{
  cs::ResourceLocator anonLocator = locator.AsAnonymous();

  auto it = m_entries.find(anonLocator);
  if (it == m_entries.end())
  {
    throw NoSuchLocatorException(locator.Encode());
  }

  int rootPriority = INT_MAX;
  for (auto entry : it->second)
  {
    try
    {
      int prio = entry->GetVFSEntryPriority();
      rootPriority = std::min<int>(rootPriority, prio);
    }
    catch (NotAttachedException &)
    {
      // ignore this
    }  }

  return rootPriority;
}


int Model::GetFinalPriority(const cs::ResourceLocator &locator) const
{
  cs::ResourceLocator anonLocator = locator.AsAnonymous();

  auto it = m_entries.find(anonLocator);
  if (it == m_entries.end())
  {
    throw NoSuchLocatorException(locator.Encode());
  }

  int finalPriority = 0;
  for (auto entry : it->second)
  {
    try
    {
      int prio = entry->GetVFSEntryPriority();
      finalPriority = std::max<int>(finalPriority, prio);
    }
    catch (NotAttachedException &)
    {
      // ignore this
    }
  }

  return finalPriority;
}


void Model::Add(asset::model::Entry *parent, asset::model::Entry *child, ModelTransaction &tr)
{
  if (!parent || !child)
  {
    return;
  }

  parent->AddChild(child);
  InsertIntoEntryCache(child);



  tr.OnCommit([this, parent, child]() {AddCommit(parent, child); });
  tr.OnRollback([this, parent, child]() {AddRollback(parent, child); });
}

void Model::AddCommit(Entry *parent, Entry *child)
{
  if (!parent || !child)
  {
    return;
  }

  emit EntryAdded(parent, child);
  emit ResourceChanged(child->GetResourceLocator().AsAnonymous());
}

void Model::AddRollback(Entry *parent, Entry *child)
{
  RemoveFromEntryCache(child);
  parent->RemoveChild(child);
}

void Model::Remove(asset::model::Entry *parent, asset::model::Entry *child, ModelTransaction &tr)
{
  if (!parent || !child || child->GetParent() != parent)
  {
    return;
  }

  cs::ResourceLocator oldLocator = child->GetResourceLocator();
  parent->RemoveChild(child);

  tr.OnCommit([this, parent, child, oldLocator]() { RemoveCommit(parent, child, oldLocator); });
  tr.OnRollback([this, parent, child]() { RemoveRollback(parent, child); });
}

void Model::RemoveCommit(asset::model::Entry *parent, asset::model::Entry *child, const cs::ResourceLocator &oldLocator)
{
  emit EntryRemoved(parent, child);
  emit ResourceChanged(oldLocator.AsAnonymous());
}

void Model::RemoveRollback(asset::model::Entry *parent, asset::model::Entry *child)
{
  parent->AddChild(child);
}


class TreeCollector
{
public:
  TreeCollector(Entry *entry)
  {
    if (entry->IsAttached())
    {
      Scan(entry);
    }
  }

  struct Data
  {
    Entry *entry;
    cs::ResourceLocator locator;
    int priority;
    int rootPriority;
    int finalPriority;
    int _pad;
  };

  void Scan(Entry *entry)
  {
    for (Entry *child : entry->GetChildren())
    {
      Scan(child);
    }

    Data d;
    d.entry = entry;
    d.locator = entry->GetResourceLocator();
    d.priority = entry->GetVFSEntryPriority();
    d.rootPriority = entry->GetModel()->GetRootPriority(d.locator);
    d.finalPriority = entry->GetModel()->GetFinalPriority(d.locator);

    m_entries.push_back(d);
  }

  std::vector<Data> m_entries;
};


void Model::Move(Entry *oldParent, Entry *newParent, Entry *child, ModelTransaction &tr)
{
  if (!oldParent || !newParent || !child || oldParent == newParent)
  {
    return;
  }
  TreeCollector treeCollector(child);
  cs::ResourceLocator oldLocator = child->GetResourceLocator();
  cs::ResourceLocator newLocator = newParent->CreateResourceLocator(child->GetName());

  SecureFS secureFS(tr);
  try
  {
    secureFS.Move(oldLocator, newLocator);
  }
  catch (const std::exception &)
  {
    throw ModelStateException("Unable to move " + oldLocator.Encode() + " to " + newLocator.Encode());
  }

  MergePath(oldParent, newParent, child, tr);
  tr.OnCommit([this, treeCollector]() {MoveCommit(treeCollector); });
}

void Model::MoveCommit(TreeCollector treeCollector)
{
  UpdateCollector(treeCollector);
}

void Model::MergePath(Entry *oldParent, Entry *newParent, Entry *child, ModelTransaction &tr)
{
  if (!oldParent || !newParent || !child || oldParent == newParent)
  {
    return;
  }
  if (child->IsVFSEntry() || child->IsRoot())
  {
    return;
  }

  if (child->IsAsset())
  {
    Remove(oldParent, child, tr);
    Add(newParent, child, tr);
  }
  else if (child->IsFolder())
  {
    Entry *existingEntry = newParent->GetChildByName(child->GetName());
    if (!existingEntry)
    {
      Remove(oldParent, child, tr);
      Add(newParent, child, tr);
    }
    else
    {
      std::vector<Entry*> children = child->GetChildren();
      for (Entry *childEntry : children)
      {
        MergePath(child, existingEntry, childEntry, tr);
      }
      Remove(oldParent, child, tr);
    }
  }
}


void Model::Delete(Entry *entry, ModelTransaction &tr)
{
  for (Entry *child : entry->GetChildren())
  {
    child->Delete(tr);
  }


  cs::ResourceLocator locator = entry->GetResourceLocator();
  tr.OnCommit([this, entry, locator]() { DeleteCommit(entry, locator); });
  entry->RemoveFromParent(tr);

  try
  {
    SecureFS secureFS(tr);
    secureFS.Delete(locator);
  }
  catch (const std::exception &e)
  {
    std::cout << "asset::model::Model: Unable to delete " << locator.Encode() << ": " << e.what() << std::endl;
    throw ModelStateException("Unable to delete " + locator.Encode());
  }

}

void Model::DeleteCommit(Entry *entry, const cs::ResourceLocator &locator)
{
  printf("DeleteCommit: %s <- %s\n",
         entry->GetName().c_str(),
         locator.Encode().c_str());
  if (!locator.IsValid())
  {
    return;
  }

  cs::ResourceLocator anonLocator = locator.AsAnonymous();
  emit EntryDeleted(entry, locator);
  if (RemoveFromEntryCache(entry))
  {
    emit ResourceRemoved(anonLocator);
    RemoveReference(anonLocator);
  }
}


void Model::Rename(Entry *entry, const std::string &name, ModelTransaction &tr)
{
  if (!entry->IsAttached())
  {
    return;
  }

  TreeCollector treeCollector(entry);

  cs::ResourceLocator oldLocator = entry->GetResourceLocator();
  cs::ResourceLocator newLocator = entry->GetParent()->CreateResourceLocator(entry->FakeName(name));

  try
  {
    SecureFS secureFS(tr);
    secureFS.Move(oldLocator, newLocator);
  }
  catch (const std::exception &)
  {
    throw ModelStateException(std::string("Unable to move ") + oldLocator.Encode() + std::string(" to ") + newLocator.Encode());
  }

  tr.OnCommit([this, entry, name, treeCollector]() { RenameCommit(entry, name, treeCollector); });
}

void Model::RenameCommit(Entry *entry, const std::string &name, TreeCollector treeCollector)
{
  entry->SetName(name);
  UpdateCollector(treeCollector);
}

void Model::UpdateCollector(TreeCollector &treeCollector)
{
  // rename the cache
  for (TreeCollector::Data &data : treeCollector.m_entries)
  {
    cs::ResourceLocator oldLocator = data.locator;
    cs::ResourceLocator newLocator = data.entry->GetResourceLocator();

    RenameCache(data.entry, oldLocator, newLocator);
    emit EntryRenamed(data.entry, oldLocator, newLocator);
  }

  // now check if the root entries are modified... in this case emit a resource renaming
  for (TreeCollector::Data &data : treeCollector.m_entries)
  {
    if (data.priority <= data.rootPriority)
    {
      cs::ResourceLocator oldAnonLocator = data.locator.AsAnonymous();
      cs::ResourceLocator newLocator = data.entry->GetResourceLocator();
      if (!data.entry->IsAttached() || !newLocator.IsValid())
      {
        emit ResourceRemoved(oldAnonLocator);
        RemoveReference(oldAnonLocator);
      }
      else
      {
        cs::ResourceLocator newAnonLocator = newLocator.AsAnonymous();
        emit ResourceRenamed(oldAnonLocator, newAnonLocator);
        RenameReference(oldAnonLocator, newAnonLocator);
        AssetRenamedEvent evt (oldAnonLocator, newAnonLocator);
        EventBus::Get().Fire(evt);
      }
    }
  }

  for (TreeCollector::Data &data : treeCollector.m_entries)
  {
    cs::ResourceLocator oldLocator = data.locator;
    cs::ResourceLocator newLocator = data.entry->GetResourceLocator();
    emit ResourceChanged(oldLocator);
    emit ResourceChanged(newLocator);
  }

}

void Model::RenameCache(Entry *entry, const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator)
{
  auto itNE = m_namedEntries.find(oldLocator);
  if (itNE != m_namedEntries.end())
  {
    m_namedEntries.erase(itNE);
  }
  if (newLocator.IsValid())
  {
    m_namedEntries[newLocator] = entry;
  }


  auto itE = m_entries.find(oldLocator.AsAnonymous());
  if (itE != m_entries.end())
  {
    auto itEE = itE->second.find(entry);
    if (itEE != itE->second.end())
    {
      itE->second.erase(itEE);
    }
    if (itE->second.empty())
    {
      m_entries.erase(itE);
    }
  }

  m_entries[newLocator.AsAnonymous()].insert(entry);

}


void Model::InsertIntoEntryCache(Entry *entry)
{
  if (!entry)
  {
    return;
  }

  cs::ResourceLocator locator = entry->GetResourceLocator();
  if (!locator.IsValid())
  {
    return;
  }

  m_namedEntries[locator] = entry;
  m_entries[locator.AsAnonymous()].insert(entry);
}

bool Model::RemoveFromEntryCache(Entry *entry)
{
  bool removedLastEntry = false;
  if (!entry)
  {
    return removedLastEntry;
  }

  cs::ResourceLocator locator = EvaluateEntryLocator(entry);
  if (!locator.IsValid())
  {
    return removedLastEntry;
  }

  auto itNE = m_namedEntries.find(locator);
  if (itNE != m_namedEntries.end())
  {
    m_namedEntries.erase(itNE);
  }

  auto itE = m_entries.find(locator.AsAnonymous());
  if (itE != m_entries.end())
  {
    auto itEE = itE->second.find(entry);
    if (itEE != itE->second.end())
    {
      itE->second.erase(itEE);
      if (itE->second.empty())
      {
        m_entries.erase(itE);
        removedLastEntry = true;
      }
    }
  }
  return removedLastEntry;
}


void Model::RemoveReference(const cs::ResourceLocator &locator)
{
  cs::ResourceLocator anonLocator = locator.AsAnonymous();
  std::set<Entry*> referencingEntries = GetReferencing(anonLocator);

  for (Entry *entry : referencingEntries)
  {
    Asset *asset = entry->AsAsset();
    if (asset)
    {
      AssetModifier modifier(asset);
      modifier.ClearLocator(anonLocator);
    }
    // reset the reference in all asset-files
  }

  for (auto it = m_references.begin(); it != m_references.end(); ++it)
  {
    auto itR = it->second.find(anonLocator);
    if (itR != it->second.end())
    {
      it->second.erase(itR);
    }
  }
}

void Model::RenameReference(const cs::ResourceLocator &olLocator, const cs::ResourceLocator &newLocator)
{
  cs::ResourceLocator oldAnonLocator = olLocator.AsAnonymous();
  cs::ResourceLocator newAnonLocator = newLocator.AsAnonymous();
  std::set<Entry*> referencingEntries = GetReferencing(oldAnonLocator);

  for (Entry *entry : referencingEntries)
  {
    Asset *asset = entry->AsAsset();
    if (asset)
    {
      AssetModifier modifier(asset);
      modifier.ReplaceLocator(oldAnonLocator, newAnonLocator);
    }
    // reset the reference in all asset-files
  }

  for (auto it = m_references.begin(); it != m_references.end(); ++it)
  {
    auto itR = it->second.find(oldAnonLocator);
    if (itR != it->second.end())
    {
      it->second.erase(itR);
      it->second.insert(newLocator);
    }
  }
}

cs::ResourceLocator Model::EvaluateEntryLocator(const Entry *entry) const
{
  cs::ResourceLocator locator = entry->GetResourceLocator();
  if (locator.IsValid())
  {
    return locator;
  }

  for (auto namedEntry : m_namedEntries)
  {
    if (namedEntry.second == entry)
    {
      return namedEntry.first;
    }
  }

  return cs::ResourceLocator();
}


void Model::ClearReferences(Entry *entry)
{
  if (!entry)
  {
    return;
  }
  auto it = m_references.find(entry);
  if (it != m_references.end())
  {
    m_references.erase(it);
  }
}

void Model::AddReference(Entry *entry, const cs::ResourceLocator &reference)
{
  if (!entry)
  {
    return;
  }

  m_references[entry].insert(reference.AsAnonymous());

}

}
