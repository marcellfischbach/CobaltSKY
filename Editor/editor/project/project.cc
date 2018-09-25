
#include <editor/project/project.hh>
#include <editor/assetmanager/assetmanagerresourcescanner.hh>
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
  m_model = new asset::model::Model();
  asset::model::ModelScanner(m_model).Scan();
}

void Project::Close()
{
}



