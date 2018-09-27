
#include <imageimporter/imageimportermodule.hh>
#include <imageimporter/imageimporterfactory.hh>
#include <imageimporter/master.refl.cc>

#include <editor/importerfactorymanager.hh>


void ImageImporterModule::Initialize(int argc, char **argv)
{
  register_classes();

  ImporterFactoryManager *importerFactoryManager = ImporterFactoryManager::Get();
  importerFactoryManager->AddImporterFactory(new ImageImporterFactory());
}

