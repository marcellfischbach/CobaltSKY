
#include <project/projectreferencetree.hh>
#include <project/projectassetreference.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <events/assetaddedevent.hh>
#include <events/assetrenamedevent.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/cseventbus.hh>
#include <cobalt/core/csfileinfo.hh>
#include <csfile/csffile.hh>
#include <eventbus.hh>
#include <QDir>
#include <QDomDocument>
#include <QFile>


ProjectReferenceTree::ProjectReferenceTree()
{
}


ProjectReferenceTree::~ProjectReferenceTree()
{
}

void ProjectReferenceTree::Open(const std::string &projectPath)
{
  m_projectPath = projectPath;

  LoadReferenceTree();
}

void ProjectReferenceTree::Close()
{
  StoreReferenceTree();
}

void ProjectReferenceTree::Clear()
{
  for (auto entry : m_allReferences)
  {
    delete entry.second;
  }
  m_references.clear();
  m_allReferences.clear();
}

void ProjectReferenceTree::LoadReferenceTree()
{
  std::string referenceTreePath = m_projectPath + "/.project/referenceTree.csf";

  csfFile file;
  if (!file.Parse(referenceTreePath))
  {
    Rebuild();
    return;
  }


  const csfEntry *referenceTreeEntry = file.GetEntry("referenceTree");
  if (!referenceTreeEntry)
  {
    Rebuild();
    return;
  }

  Clear();
  const csfEntry *assetsEntry = referenceTreeEntry->GetEntry("assets");
  if (assetsEntry)
  {
    for (const csfEntry *assetEntry = assetsEntry->GetEntry("asset");
      assetEntry;
      assetEntry = assetEntry->GetSiblingEntry("asset"))
    {
      if (!assetEntry->HasAttribute("locator")
        || !assetEntry->HasAttribute("name")
        || !assetEntry->HasAttribute("type"))
      {
        continue;
      }
      std::string resourceLocator = assetEntry->GetAttribute("locator");
      std::string name = assetEntry->GetAttribute("name");
      std::string type = assetEntry->GetAttribute("type");
      unsigned priority = 0;
      if (assetEntry->HasAttribute("priority"))
      {
        priority = assetEntry->GetAttributeInt("priority");
      }

      ProjectAssetReference *assetReference = new ProjectAssetReference();
      assetReference->SetName(name);
      assetReference->SetResourceLocator(csResourceLocator(resourceLocator));
      assetReference->SetTypeName(type);
      assetReference->SetPriority(priority);
      m_allReferences[assetReference->GetResourceLocator()] = assetReference;

    }
  }

  RebuildTree();

  const csfEntry *referencesEntry = referenceTreeEntry->GetEntry("references");
  if (referencesEntry)
  {
    for (const csfEntry *referenceEntry = referencesEntry->GetEntry("reference");
      referenceEntry;
      referenceEntry = referenceEntry->GetSiblingEntry("reference"))
    {
      if (!referenceEntry->HasAttribute("asset") ||
        !referenceEntry->HasAttribute("references"))
      {
        continue;
      }

      std::string assetResourceLocator = referenceEntry->GetAttribute("asset");
      std::string referencesResourceLocator = referenceEntry->GetAttribute("references");

      ProjectAssetReference *asset = m_allReferences[csResourceLocator(assetResourceLocator)];
      ProjectAssetReference *reference = m_references[csResourceLocator(referencesResourceLocator)];

      if (asset && reference)
      {
        asset->AddReference(reference);
        while (reference)
        {
          reference->AddReferencedBy(asset);
          reference = reference->GetChild();
        }
      }
    }
  }

}

void ProjectReferenceTree::StoreReferenceTree()
{

  csfFile file;
  csfEntry *referenceTreeEntry = file.CreateEntry("referenceTree");
  csfEntry *assetsEntry = file.CreateEntry("assets");
  file.GetRoot()->AddChild(referenceTreeEntry);
  referenceTreeEntry->AddChild(assetsEntry);

  for (auto entry : m_allReferences)
  {
    ProjectAssetReference *asset = entry.second;

    csfEntry *assetEntry = file.CreateEntry("asset");
    assetEntry->AddAttribute("locator", asset->GetResourceLocator().Encode());
    assetEntry->AddAttribute("name", asset->GetName());
    assetEntry->AddAttribute("type", asset->GetTypeName());
    assetEntry->AddAttributeInt("priority", asset->GetPriority());
    assetsEntry->AddChild(assetEntry);
  }


  csfEntry *referencesEntry = file.CreateEntry("references");
  referenceTreeEntry->AddChild(referencesEntry);
  for (auto entry : m_allReferences)
  {
    ProjectAssetReference *asset = entry.second;
    const std::vector<ProjectAssetReference*> references = asset->GetReferences();

    for (auto reference : references)
    {
      csfEntry *referenceEntry = file.CreateEntry("reference");
      referenceEntry->AddAttribute("asset", asset->GetResourceLocator().Encode());
      referenceEntry->AddAttribute("references", reference->GetResourceLocator().AsAnonymous().Encode());
      referencesEntry->AddChild(referenceEntry);
    }
  }

  std::string referenceTreePath = m_projectPath + "/.project/referenceTree.csf";
  file.Output(referenceTreePath, false, 2);
}

void ProjectReferenceTree::Rebuild()
{
  printf("Rebuild reference tree\n");
  AssetManagerResourceScanner scanner;
  scanner.ScanPath();

  const std::vector<AssetManagerResourceScanner::Entry> allEntries = scanner.GetAllEntries();
  Clear();

  for (auto entry : allEntries)
  {
    ProjectAssetReference *assetReference = new ProjectAssetReference();
    assetReference->SetName(entry.name);
    assetReference->SetResourceLocator(entry.locator);
    assetReference->SetTypeName(entry.typeName);
    assetReference->SetPriority(entry.priority);
    m_allReferences[entry.locator] = assetReference;
  }

  RebuildTree();

  for (auto entry : allEntries)
  {
    ProjectAssetReference *asset = m_allReferences[entry.locator];
    if (!asset)
    {
      continue;
    }


    for (auto referenceLocator : entry.references)
    {
      ProjectAssetReference *reference = m_references[referenceLocator];
      if (reference)
      {
        asset->AddReference(reference);
        while (reference)
        {
          reference->AddReferencedBy(asset);
          reference = reference->GetChild();
        }
      }
    }
  }
  printf("Rebuild dependency tree - done\n");
  StoreReferenceTree();
}

void ProjectReferenceTree::RebuildTree()
{
  m_references.clear();
  for (auto ref : m_allReferences)
  {
    ref.second->SetChild(0);
    ref.second->SetParent(0);
    csResourceLocator anonLoc = ref.first.AsAnonymous();
    ProjectAssetReference *currentReference = m_references[anonLoc];
    if (!currentReference)
    {
      m_references[anonLoc] = ref.second;
    }
    else
    {
      m_references[anonLoc] = currentReference->InsertChild(ref.second);
    }
  }
}

void ProjectReferenceTree::UpdateDependencyTree(const csResourceLocator &resourceLocator)
{

  ProjectAssetReference *assetReference = m_references[resourceLocator];
  if (!assetReference)
  {
    Rebuild();
    return;
  }

  for (auto reference : assetReference->GetReferences())
  {
    reference->RemoveReferencedBy(reference);
  }
  assetReference->ClearReference();



  AssetManagerResourceScanner scanner;
  for (auto it : m_references)
  {
    ProjectAssetReference *ref = it.second;
    scanner.AddEntry(ref->GetResourceLocator(), ref->GetName(), ref->GetTypeName());
  }

  scanner.ScanReference(resourceLocator);
  const std::vector<AssetManagerResourceScanner::Entry> &entries = scanner.GetAllEntries();
  for (auto entry : entries)
  {
    if (entry.locator == resourceLocator)
    {
      for (auto ref : entry.references)
      {
        ProjectAssetReference *reference = m_references[ref];
        if (assetReference && reference)
        {
          assetReference->AddReference(reference);
          reference->AddReferencedBy(assetReference);
        }
      }
      break;
    }
  }
  StoreReferenceTree();
}

void ProjectReferenceTree::Rename(const csResourceLocator &from, const csResourceLocator &to)
{
  ProjectAssetReference *reference = 0;
  if (from.IsAnonymous())
  {
    std::map<csResourceLocator, ProjectAssetReference*>::iterator it = m_references.find(from);
    if (it != m_references.end())
    {
      reference = it->second;
      if (reference)
      {
        // exchange the name it the map
        m_references.erase(it);
        m_references[to.AsAnonymous()] = reference;
      }
    }
  }
  else
  {
    std::map<csResourceLocator, ProjectAssetReference*>::iterator it = m_allReferences.find(from);
    if (it != m_allReferences.end())
    {
      reference = it->second;
    }
  }

  while (reference)
  {
    csResourceLocator oldLocator = reference->GetResourceLocator();
    csResourceLocator newLocator = csResourceLocator(to.GetResourceFile(), oldLocator.GetResourceName(), oldLocator.GetResourceEntry());
    reference->SetResourceLocator(newLocator);

    csFileInfo fi(newLocator.GetResourceFile());
    reference->SetName(fi.GetName());

    std::map<csResourceLocator, ProjectAssetReference*>::iterator it = m_allReferences.find(oldLocator);
    m_allReferences.erase(it);
    m_allReferences[newLocator] = reference;

    // exchange this all the way up
    reference = reference->GetChild();
  }


  // TODO: Maybe update the reference tree on the point... not just regenerate
  RebuildTree();
  StoreReferenceTree();
}


const ProjectAssetReference *ProjectReferenceTree::GetReference(const csResourceLocator &resource) const
{
  if (resource.IsAnonymous())
  {
    auto it = m_references.find(resource);
    if (it != m_references.end())
    {
      return it->second;
    }
  }
  else
  {
    auto it = m_allReferences.find(resource);
    if (it != m_allReferences.end())
    {
      return it->second;
    }
  }
  return 0;
}


void ProjectReferenceTree::Debug()
{
  printf("ReferenceTree\n");
  for (auto entry : m_references)
  {
    Debug(entry.second, 1);
  }
}

static void ind(int indent)
{
  for (int i = 0; i < indent; ++i)
  {
    printf("  ");
  }
}

void ProjectReferenceTree::Debug(ProjectAssetReference *reference, int indent)
{
  if (reference)
  {
    ind(indent);
    printf("Locator: %s  Priority: %d\n", reference->GetResourceLocator().GetDebugName().c_str(), reference->GetPriority());
    Debug(reference->GetChild(), indent + 1);
  }
}

