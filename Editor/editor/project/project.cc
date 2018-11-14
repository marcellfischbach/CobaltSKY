
#include <editor/project/project.hh>
#include <editor/assetmanager/assetmanagerresourcescanner.hh>
#include <QDomDocument>
#include <QFile>
#include <editor/assetmodel/model.hh>
#include <editor/assetmodel/sync/modelscanner.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <iostream>

Project::Project()
  : m_model(0)
{

}

void Project::Open(const std::string &projectPath)
{
  m_model = new asset::model::Model();
  connect (m_model, SIGNAL(ResourceChanged(const csResourceLocator&)), this, SLOT(ResourceChanged(const csResourceLocator&)));
  asset::model::ModelScanner(m_model).Scan();
}

void Project::Close()
{
}


void Project::ResourceChanged(const csResourceLocator &locator)
{
  std::cout << "ResourceChanged: " << locator.Encode() << "\n";
  fflush(stdout);
  csResourceManager::Get()->Reload(locator.AsAnonymous());
}


