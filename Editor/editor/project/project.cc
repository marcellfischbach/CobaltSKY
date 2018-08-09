
#include <editor/project/project.hh>
#include <editor/project/projectmodel.hh>
#include <editor/project/projectmodelscanner.hh>
#include <editor/assetmanager/assetmanagerresourcescanner.hh>
#include <editor/assetmanager/assetmanagerrenamer.hh>
#include <QDomDocument>
#include <QFile>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/sync/modelscanner.hh>

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