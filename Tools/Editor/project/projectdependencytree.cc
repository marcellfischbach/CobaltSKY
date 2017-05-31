
#include <project/projectdependencytree.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <QDomDocument>
#include <QFile>

ProjectDependencyTree::ProjectDependencyTree()
{

}

void ProjectDependencyTree::Open(const std::string &projectPath)
{
  m_projectPath = projectPath;

  LoadDependencyTree();
  printf("Project opened\n");
}

void ProjectDependencyTree::Close()
{
  StoreDependencyTree();
}

void ProjectDependencyTree::LoadDependencyTree()
{
  std::string referenceTreePath = m_projectPath + "/.project/dependencyTree.xml";

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
  m_referenceDependencies.clear();

  QDomElement element = doc.documentElement();
  if (element.tagName() == QString("dependencyTree"))
  {
    QDomElement resourcesElement = element.firstChildElement("resources");
    if (!resourcesElement.isNull())
    {
      for (QDomElement resourceElement = resourcesElement.firstChildElement("resource");
        !resourceElement.isNull();
        resourceElement = resourceElement.nextSiblingElement("resource"))
      {
        if (resourceElement.hasAttribute("asset"))
        {
          std::string asset = std::string((const char*)resourceElement.attribute("asset").toLatin1());

          m_allResources.insert(asset);
        }
      }
    }
    QDomElement entriesElement = element.firstChildElement("dependencies");
    if (!entriesElement.isNull())
    {
      for (QDomElement entryElement = entriesElement.firstChildElement("dependency");
        !entryElement.isNull();
        entryElement = entryElement.nextSiblingElement("dependency"))
      {
        if (entryElement.hasAttribute("asset") && entryElement.hasAttribute("dependsOn"))
        {
          std::string asset = std::string((const char*)entryElement.attribute("asset").toLatin1());
          std::string dependsOn = std::string((const char*)entryElement.attribute("dependsOn").toLatin1());

          m_referenceDependencies[asset].insert(dependsOn);
          m_references[dependsOn].insert(asset);
        }
      }
    }
  }
}

void ProjectDependencyTree::StoreDependencyTree()
{
  QDomDocument doc;

  QDomElement referenceTreeElement = doc.createElement("dependencyTree");
  QDomElement resourcesElement = doc.createElement("resources");
  referenceTreeElement.appendChild(resourcesElement);
  for (auto resource : m_allResources)
  {
    QDomElement resourceElement = doc.createElement("resource");
    resourceElement.setAttribute("asset", QString(resource.c_str()));
    resourcesElement.appendChild(resourceElement);

  }

  QDomElement entriesElement = doc.createElement("dependencies");
  referenceTreeElement.appendChild(entriesElement);
  for (auto ref : m_referenceDependencies)
  {
    for (auto dep : ref.second)
    {
      QDomElement dependencyElement = doc.createElement("dependency");
      dependencyElement.setAttribute("asset", QString(ref.first.c_str()));
      dependencyElement.setAttribute("dependsOn", QString(dep.c_str()));
      entriesElement.appendChild(dependencyElement);
    }
  }
  doc.appendChild(referenceTreeElement);

  std::string referenceTreePath = m_projectPath + "/.project/dependencyTree.xml";
  QFile file(QString(referenceTreePath.c_str()));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(doc.toString(2).toLatin1());
    file.close();
  }


}

void ProjectDependencyTree::RebuildDependencyTree()
{
  printf("Rebuild dependency tree\n");
  AssetManagerResourceScanner scanner;
  scanner.Scan();
  const std::set<std::string> &allResources = scanner.GetAllResources();
  const std::set<std::pair<std::string, std::string>> &dependencies = scanner.GetDependencies();
  m_allResources.clear();
  m_references.clear();
  m_referenceDependencies.clear();
  for (auto resource : allResources)
  {
    m_allResources.insert(resource);
  }
  for (auto dependency : dependencies)
  {
    m_referenceDependencies[dependency.first].insert(dependency.second);
    m_references[dependency.second].insert(dependency.first);
  }

  printf("Rebuild dependency tree - done\n");
  StoreDependencyTree();
}

void ProjectDependencyTree::UpdateDependencyTree(const std::string &resourceName)
{
  m_referenceDependencies.erase(resourceName);

  for (auto it : m_references)
  {
    it.second.erase(resourceName);
  }

  AssetManagerResourceScanner scanner;
  for (auto it : m_allResources)
  {
    scanner.AddResourceName(it);
  }

  scanner.ScanReference(resourceName);
  const std::set<std::pair<std::string, std::string>> &dependencies = scanner.GetDependencies();
  for (auto dependency : dependencies)
  {
    m_referenceDependencies[dependency.first].insert(dependency.second);
    m_references[dependency.second].insert(dependency.first);
  }
  StoreDependencyTree();
}