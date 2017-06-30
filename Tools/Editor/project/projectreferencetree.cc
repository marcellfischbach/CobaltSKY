
#include <project/projectreferencetree.hh>
#include <project/projectassetreference.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <QDomDocument>
#include <QFile>

ProjectReferenceTree::ProjectReferenceTree()
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
  for (auto entry : m_references)
  {
    if (entry.second)
    {
      delete entry.second;
    }
  }
  m_references.clear();

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


      ProjectAssetReference *assetReference = new ProjectAssetReference();
      assetReference->SetName(name);
      assetReference->SetResourceLocator(csResourceLocator(resourceLocator));
      assetReference->SetTypeName(type);
      m_references[resourceLocator] = assetReference;
    }
  }

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

      ProjectAssetReference *asset = m_references[csResourceLocator(assetResourceLocator)];
      ProjectAssetReference *reference = m_references[csResourceLocator(assetResourceLocator)];

      if (asset && reference)
      {
        asset->AddReference(reference);
        reference->AddReferencedBy(asset);
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
  for (auto entry : m_references)
  {
    ProjectAssetReference *asset = entry.second;

    QDomElement assetElement = doc.createElement("asset");
    assetElement.setAttribute("locator", QString(asset->GetResourceLocator().GetText().c_str()));
    assetElement.setAttribute("name", QString(asset->GetName().c_str()));
    assetElement.setAttribute("type", QString(asset->GetTypeName().c_str()));
    assetsElement.appendChild(assetElement);
  }



  QDomElement referencesElement = doc.createElement("references");
  referenceTreeElement.appendChild(referencesElement);
  for (auto entry : m_references)
  {
    ProjectAssetReference *asset = entry.second;
    const std::vector<ProjectAssetReference*> references = asset->GetReferences();

    for (auto reference : references)
    {
      QDomElement referenceElement = doc.createElement("reference");
      referenceElement.setAttribute("asset", QString(asset->GetResourceLocator().GetText().c_str()));
      referenceElement.setAttribute("references", QString(reference->GetResourceLocator().GetText().c_str()));
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
    m_references[entry.locator] = assetReference;
  }

  for (auto entry : allEntries)
  {
    ProjectAssetReference *asset = m_references[entry.locator];
    for (auto referenceLocator : entry.references)
    {
      ProjectAssetReference *reference = m_references[referenceLocator];
      if (asset && reference)
      {
        asset->AddReference(reference);
        reference->AddReferencedBy(asset);
      }
    }
  }
  printf("Rebuild dependency tree - done\n");
  StoreReferenceTree();
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
  auto it = m_references.find(from);
  if (it == m_references.end())
  {
    return;
  }
  ProjectAssetReference *assetReference = it->second;
  assetReference->SetResourceLocator(to);
  m_references.erase(it);
  m_references[to] = assetReference;

  StoreReferenceTree();
}


const ProjectAssetReference *ProjectReferenceTree::GetReference(const csResourceLocator &resource) const
{
  auto it = m_references.find(resource);
  if (it == m_references.end())
  {
    return 0;
  }
  return it->second;
}
