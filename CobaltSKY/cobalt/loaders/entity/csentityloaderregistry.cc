
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/loaders/entity/csentityxmlloader.hh>
#include <cobalt/loaders/entity/csentitystatexmlloader.hh>




csEntityLoaderRegistry::csEntityLoaderRegistry()
{
}

csEntityLoaderRegistry *csEntityLoaderRegistry::Get()
{
  static csEntityLoaderRegistry static_instance;
  return &static_instance;
}


void csEntityLoaderRegistry::RegisterLoader(const csEntityXMLLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityLoaders[loader->GetLoadingClass()] = loader;
}


void csEntityLoaderRegistry::RegisterLoader(const csEntityStateXMLLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityStateLoaders[loader->GetLoadingClass()] = loader;
}


const csEntityXMLLoader *csEntityLoaderRegistry::GetEntityLoader(const csClass *clazz) const
{
  std::map<const csClass *, const csEntityXMLLoader*>::const_iterator it = m_entityLoaders.find(clazz);

  if (it != m_entityLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const csClass *clazz1 = clazz->GetSuperClass(i);
    const csEntityXMLLoader *loader = GetEntityLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}



const csEntityStateXMLLoader *csEntityLoaderRegistry::GetEntityStateLoader(const csClass *clazz) const
{
  std::map<const csClass *, const csEntityStateXMLLoader*>::const_iterator it = m_entityStateLoaders.find(clazz);

  if (it != m_entityStateLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const csClass *clazz1 = clazz->GetSuperClass(i);
    const csEntityStateXMLLoader *loader = GetEntityStateLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}
