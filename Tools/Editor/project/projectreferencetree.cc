
#include <project/projectreferencetree.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <QDomDocument>
#include <QFile>

ProjectReferenceTree::ProjectReferenceTree()
{

}

void ProjectReferenceTree::Open(const std::string &projectPath)
{
  m_projectPath = projectPath;

  LoadDependencyTree();
  printf("Project opened\n");
}

void ProjectReferenceTree::Close()
{
  StoreDependencyTree();
}

void ProjectReferenceTree::LoadDependencyTree()
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

  m_references.clear();
  m_referencedBy.clear();

  QDomElement element = doc.documentElement();
  if (element.tagName() == QString("referenceTree"))
  {
    QDomElement resourcesElement = element.firstChildElement("resources");
    if (!resourcesElement.isNull())
    {
      for (QDomElement resourceElement = resourcesElement.firstChildElement("resource");
        !resourceElement.isNull();
        resourceElement = resourceElement.nextSiblingElement("resource"))
      {
        if (resourceElement.hasAttribute("locator"))
        {
          std::string resourceLocator = std::string((const char*)resourceElement.attribute("locator").toLatin1());
          m_allResources.insert(vkResourceLocator(resourceLocator));
        }
      }
    }


    QDomElement referencesElement = element.firstChildElement("references");
    if (!referencesElement.isNull())
    {
      for (QDomElement referenceElement = referencesElement.firstChildElement("reference");
        !referenceElement.isNull();
        referenceElement = referenceElement.nextSiblingElement("reference"))
      {
        if (referenceElement.hasAttribute("resource") && referenceElement.hasAttribute("references"))
        {
          std::string resource = std::string((const char*)referenceElement.attribute("resource").toLatin1());
          std::string references= std::string((const char*)referenceElement.attribute("references").toLatin1());

          vkResourceLocator resourceLocator(resource);
          vkResourceLocator referencesLocator(references);

          m_referencedBy[referencesLocator].insert(resourceLocator);
          m_references[resourceLocator].insert(referencesLocator);
        }
      }
    }
  }
}

void ProjectReferenceTree::StoreDependencyTree()
{
  QDomDocument doc;

  QDomElement referenceTreeElement = doc.createElement("referenceTree");
  QDomElement resourcesElement = doc.createElement("resources");
referenceTreeElement.appendChild(resourcesElement);
for (auto resource : m_allResources)
{
  QDomElement resourceElement = doc.createElement("resource");
  resourceElement.setAttribute("locator", QString(resource.GetText().c_str()));
  resourcesElement.appendChild(resourceElement);

}

QDomElement referencesElement = doc.createElement("references");
referenceTreeElement.appendChild(referencesElement);
for (auto resource : m_references)
{
  for (auto reference : resource.second)
  {
    QDomElement dependencyElement = doc.createElement("reference");
    dependencyElement.setAttribute("resource", QString(resource.first.GetText().c_str()));
    dependencyElement.setAttribute("references", QString(reference.GetText().c_str()));
    referencesElement.appendChild(dependencyElement);
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

void ProjectReferenceTree::RebuildDependencyTree()
{
  printf("Rebuild dependency tree\n");
  AssetManagerResourceScanner scanner;
  scanner.Scan();
  const std::set<vkResourceLocator> &allResources = scanner.GetAllResources();
  const std::set<std::pair<vkResourceLocator, vkResourceLocator>> &references = scanner.GetReferences();
  m_allResources.clear();
  m_references.clear();
  m_referencedBy.clear();
  for (auto resource : allResources)
  {
    m_allResources.insert(resource);
  }
  for (auto references : references)
  {
    m_referencedBy[references.second].insert(references.first);
    m_references[references.first].insert(references.second);
  }

  printf("Rebuild dependency tree - done\n");
  StoreDependencyTree();
}

void ProjectReferenceTree::UpdateDependencyTree(const vkResourceLocator &resourceLocator)
{
  m_references.erase(resourceLocator);

  for (auto it : m_referencedBy)
  {
    it.second.erase(resourceLocator);
  }

  AssetManagerResourceScanner scanner;
  for (auto it : m_allResources)
  {
    scanner.AddResource(it);
  }

  scanner.ScanReference(resourceLocator);
  const std::set<std::pair<vkResourceLocator, vkResourceLocator>> &references = scanner.GetReferences();
  for (auto reference : references)
  {
    m_referencedBy[reference.second].insert(reference.first);
    m_references[reference.first].insert(reference.second);
  }
  StoreDependencyTree();
}

void ProjectReferenceTree::Rename(const vkResourceLocator &from, const vkResourceLocator &to)
{
  if (m_allResources.find(from) != m_allResources.end())
  {
    m_allResources.erase(from);
    m_allResources.insert(to);
  }
  Rename(m_references, from, to);
  Rename(m_referencedBy, from, to);
  StoreDependencyTree();
}

void ProjectReferenceTree::Rename(std::map<vkResourceLocator, std::set<vkResourceLocator>> &references, const vkResourceLocator &from, const vkResourceLocator &to)
{
  std::map<vkResourceLocator, std::set<vkResourceLocator>>::iterator it = references.find(from);
  if (it != references.end())
  {
    std::set<vkResourceLocator> res = it->second;
    references.erase(it);
    references[to] = res;
  }

  for (it = references.begin(); it != references.end(); ++it)
  {
    if (it->second.find(from) != it->second.end())
    {
      it->second.erase(from);
      it->second.insert(to);
    }
  }
}

std::set<vkResourceLocator> ProjectReferenceTree::GetReference(const vkResourceLocator &resource) const
{
  std::map<vkResourceLocator, std::set<vkResourceLocator>>::const_iterator it = m_references.find(resource);
  if (it == m_references.end())
  {
    return std::set<vkResourceLocator>();
  }
  return it->second;
}

std::set<vkResourceLocator> ProjectReferenceTree::GetReferencedBy(const vkResourceLocator &resource) const
{
  std::map<vkResourceLocator, std::set<vkResourceLocator>>::const_iterator it = m_referencedBy.find(resource);
  if (it == m_referencedBy.end())
  {
    return std::set<vkResourceLocator>();
  }
  return it->second;
}
