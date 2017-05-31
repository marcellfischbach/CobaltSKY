
#include <project/project.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
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


ProjectDependencyTree &Project::GetDependencyTree()
{
  return m_dependencyTree;
}

const ProjectDependencyTree &Project::GetDependencyTree() const
{
  return m_dependencyTree;
}