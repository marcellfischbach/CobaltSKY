
#include <project/project.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <assetmanager/assetmanagerrenamer.hh>
#include <QDomDocument>
#include <QFile>

Project::Project()
{

}

void Project::Open(const std::string &projectPath)
{
  m_dependencyTree.Open(projectPath);
  printf("Project opened\n");
}

void Project::Close()
{
  m_dependencyTree.Close();
  printf("Project closed\n");
}


ProjectReferenceTree &Project::GetDependencyTree()
{
  return m_dependencyTree;
}

const ProjectReferenceTree &Project::GetDependencyTree() const
{
  return m_dependencyTree;
}

bool Project::Rename(const csResourceLocator &from, const csResourceLocator &to)
{
  AssetManagerRenamer renamer(from, to);
  if (!renamer.Execute())
  {
    return false;
  }

  m_dependencyTree.Rename(from.AsAnonymous(), to.AsAnonymous());

  emit ResourceRenamed(from, to);
  return true;
}