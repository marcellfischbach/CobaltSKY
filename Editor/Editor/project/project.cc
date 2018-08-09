
#include <project/project.hh>
#include <project/projectmodel.hh>
#include <project/projectmodelscanner.hh>
#include <assetmanager/assetmanagerresourcescanner.hh>
#include <assetmanager/assetmanagerrenamer.hh>
#include <QDomDocument>
#include <QFile>
#include <assetmodel/model.hh>
#include <assetmodel/sync/modelscanner.hh>

Project::Project()
  : m_model(0)
{

}

void Project::Open(const std::string &projectPath)
{
  m_referenceTree.Open(projectPath);
  printf("Project opened\n");

  m_model = new asset::model::Model();
  asset::model::ModelScanner(m_model).Scan();

  ProjectModel model;
  ProjectModelScanner scanner(&model);
  scanner.ScanAll();
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