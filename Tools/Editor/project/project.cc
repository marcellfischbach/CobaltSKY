
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
  m_referenceTree.Open(projectPath);
  printf("Project opened\n");
}

void Project::Close()
{
  m_referenceTree.Close();
  printf("Project closed\n");
}


ProjectReferenceTree &Project::GetReferenceTree()
{
  return m_referenceTree;
}

const ProjectReferenceTree &Project::GetReferenceTree() const
{
  return m_referenceTree;
}

bool Project::Rename(const csResourceLocator &from, const csResourceLocator &to)
{
  AssetManagerRenamer renamer(from, to);
  return renamer.Execute();
}