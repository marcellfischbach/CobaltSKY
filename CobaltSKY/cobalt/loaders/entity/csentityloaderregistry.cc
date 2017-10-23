
#include <cobalt/loaders/entity/csentityloaderregistry.hh>
#include <cobalt/loaders/entity/csentitycsfloader.hh>
#include <cobalt/loaders/entity/csentityxmlloader.hh>
#include <cobalt/loaders/entity/csentitystatecsfloader.hh>
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

  m_entityXMLLoaders[loader->GetLoadingClass()] = loader;
}


void csEntityLoaderRegistry::RegisterLoader(const csEntityStateXMLLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityStateXMLLoaders[loader->GetLoadingClass()] = loader;
}


const csEntityXMLLoader *csEntityLoaderRegistry::GetEntityXMLLoader(const csClass *clazz) const
{
  std::map<const csClass *, const csEntityXMLLoader*>::const_iterator it = m_entityXMLLoaders.find(clazz);

  if (it != m_entityXMLLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const csClass *clazz1 = clazz->GetSuperClass(i);
    const csEntityXMLLoader *loader = GetEntityXMLLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}



const csEntityStateXMLLoader *csEntityLoaderRegistry::GetEntityStateXMLLoader(const csClass *clazz) const
{
  std::map<const csClass *, const csEntityStateXMLLoader*>::const_iterator it = m_entityStateXMLLoaders.find(clazz);

  if (it != m_entityStateXMLLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const csClass *clazz1 = clazz->GetSuperClass(i);
    const csEntityStateXMLLoader *loader = GetEntityStateXMLLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}




void csEntityLoaderRegistry::RegisterLoader(const csEntityCSFLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityCSFLoaders[loader->GetLoadingClass()] = loader;
}


void csEntityLoaderRegistry::RegisterLoader(const csEntityStateCSFLoader *loader)
{
  if (!loader)
  {
    return;
  }

  m_entityStateCSFLoaders[loader->GetLoadingClass()] = loader;
}


const csEntityCSFLoader *csEntityLoaderRegistry::GetEntityCSFLoader(const csClass *clazz) const
{
  std::map<const csClass *, const csEntityCSFLoader*>::const_iterator it = m_entityCSFLoaders.find(clazz);

  if (it != m_entityCSFLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const csClass *clazz1 = clazz->GetSuperClass(i);
    const csEntityCSFLoader *loader = GetEntityCSFLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}



const csEntityStateCSFLoader *csEntityLoaderRegistry::GetEntityStateCSFLoader(const csClass *clazz) const
{
  std::map<const csClass *, const csEntityStateCSFLoader*>::const_iterator it = m_entityStateCSFLoaders.find(clazz);

  if (it != m_entityStateCSFLoaders.end())
  {
    return it->second;
  }

  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    const csClass *clazz1 = clazz->GetSuperClass(i);
    const csEntityStateCSFLoader *loader = GetEntityStateCSFLoader(clazz1);
    if (loader)
    {
      return loader;
    }
  }
  return 0;
}
