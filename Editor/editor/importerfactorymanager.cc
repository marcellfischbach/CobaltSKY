
#include <editor/importerfactorymanager.hh>
#include <editor/iassetimporterfactory.hh>

ImporterFactoryManager::ImporterFactoryManager()
{
}


ImporterFactoryManager *ImporterFactoryManager::Get()
{
  static ImporterFactoryManager staticInstance;
  return &staticInstance;
}

void ImporterFactoryManager::AddImporterFactory(iAssetImporterFactory *factory)
{
  m_factories.push_back(factory);
}

std::vector<iAssetImporterFactory*> ImporterFactoryManager::GetFactories() const
{
  return m_factories;
}

