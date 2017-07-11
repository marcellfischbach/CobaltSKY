
#include <project/projectreferencetree.hh>
#include <project/projectassetreference.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <events/assetaddedevent.hh>
#include <events/assetrenamedevent.hh>
#include <cobalt/core/csevent.hh>
#include <cobalt/core/cseventbus.hh>
#include <cobalt/core/csfileinfo.hh>
#include <eventbus.hh>
#include <QDomDocument>
#include <QFile>


static void project_reference_tree_asset_added(csEvent &event, void *userData)
{
  AssetAddedEvent &addedEvent = static_cast<AssetAddedEvent&>(event);
  ProjectReferenceTree *tree = reinterpret_cast<ProjectReferenceTree*>(userData);
  tree->UpdateDependencyTree(addedEvent.GetLocator());
}

ProjectReferenceTree::ProjectReferenceTree()
{
  EventBus::Get().Register(AssetAddedEvent::GetStaticClass(), project_reference_tree_asset_added, this);
}


ProjectReferenceTree::~ProjectReferenceTree()
{
  EventBus::Get().Deregister(project_reference_tree_asset_added, this);
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
  std::string referenceTreePath = m_projectPath + "/.project/referenceTree.xml";
  QFile file(QString(referenceTreePath.c_str()));


  if (!file.open(QIODevice::ReadOnly))
  {
    return;
  }
  QDomDocument doc;
  if (!doc.setContent(&file))
  {
    file.close();
    return;
  }
  file.close();

  Clear();


  QDomElement element = doc.documentElement();
  if (element.tagName() != QString("referenceTree"))
  {
    Rebuild();
    return;
  }


  QDomElement assetsElement = element.firstChildElement("assets");
  if (!assetsElement.isNull())
  {
    for (QDomElement assetElement = assetsElement.firstChildElement("asset");
      !assetElement.isNull();
      assetElement = assetElement.nextSiblingElement("asset"))
    {

      if (!assetElement.hasAttribute("locator") ||
        !assetElement.hasAttribute("name") ||
        !assetElement.hasAttribute("type"))
      {
        continue;
      }

      std::string resourceLocator = std::string((const char*)assetElement.attribute("locator").toLatin1());
      std::string name = std::string((const char*)assetElement.attribute("name").toLatin1());
      std::string type = std::string((const char*)assetElement.attribute("type").toLatin1());
      unsigned priority = 0;
      if (assetElement.hasAttribute("priority"))
      {
        priority = atoi((const char*)assetElement.attribute("priority").toLatin1());
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

  QDomElement referencesElement = element.firstChildElement("references");
  if (!referencesElement.isNull())
  {
    for (QDomElement referenceElement = referencesElement.firstChildElement("reference");
      !referenceElement.isNull();
      referenceElement = referenceElement.nextSiblingElement("reference"))
    {
      if (!referenceElement.hasAttribute("asset") ||
        !referenceElement.hasAttribute("references"))
      {
        continue;
      }

      std::string assetResourceLocator = std::string((const char*)referenceElement.attribute("asset").toLatin1());
      std::string referencesResourceLocator = std::string((const char*)referenceElement.attribute("references").toLatin1());

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
  QDomDocument doc;

  QDomElement referenceTreeElement = doc.createElement("referenceTree");
  QDomElement assetsElement = doc.createElement("assets");
  referenceTreeElement.appendChild(assetsElement);
  for (auto entry : m_allReferences)
  {
    ProjectAssetReference *asset = entry.second;

    QDomElement assetElement = doc.createElement("asset");
    assetElement.setAttribute("locator", QString(asset->GetResourceLocator().GetText().c_str()));
    assetElement.setAttribute("name", QString(asset->GetName().c_str()));
    assetElement.setAttribute("type", QString(asset->GetTypeName().c_str()));
    assetElement.setAttribute("priority", asset->GetPriority());
    assetsElement.appendChild(assetElement);
  }



  QDomElement referencesElement = doc.createElement("references");
  referenceTreeElement.appendChild(referencesElement);
  for (auto entry : m_allReferences)
  {
    ProjectAssetReference *asset = entry.second;
    const std::vector<ProjectAssetReference*> references = asset->GetReferences();

    for (auto reference : references)
    {
      QDomElement referenceElement = doc.createElement("reference");
      referenceElement.setAttribute("asset", QString(asset->GetResourceLocator().GetText().c_str()));
      referenceElement.setAttribute("references", QString(reference->GetResourceLocator().AsAnonymous().GetText().c_str()));
      referencesElement.appendChild(referenceElement);
    }
  }
  doc.appendChild(referenceTreeElement);

  std::string referenceTreePath = m_projectPath + "/.project/referenceTree.xml";
  QFile file(QString(referenceTreePath.c_str()));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(doc.toString(2).toLatin1());
    file.close();
  }


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

