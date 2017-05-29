
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <valkyrie/core/vkvfs.hh>





vkResourceManager *vkResourceManager::Get()
{
  static vkResourceManager static_manager;
  return &static_manager;
}


vkResourceManager::vkResourceManager()
{
  RegisterLoader(new vkXMLFileLoader());
  RegisterLoader(new vkAssetFileLoader());
  RegisterLoader(new vkAssetXMLLoader());
}


void vkResourceManager::RegisterLoader(iFileLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_fileLoaders.push_back(loader);
  }
}

void vkResourceManager::RegisterLoader(iXMLLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_xmlLoaders.push_back(loader);
  }
}

void vkResourceManager::RegisterLoader(iAssetLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_assetLoaders.push_back(loader);
  }
}

iObject *vkResourceManager::Load(const vkResourceLocator &locator, iObject *userData) const
{
  iFile *file = vkVFS::Get()->Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  iObject *object = Load(file, locator, userData);
  file->Release();

  return object;
}

const vkClass *vkResourceManager::EvalClass(const vkResourceLocator &locator, iObject *userData) const
{
  iFile *file = vkVFS::Get()->Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  const vkClass *cls = EvalClass(file, locator, userData);
  file->Release();

  return cls;
}

iObject *vkResourceManager::Load(iFile *file, const vkResourceLocator &locator, iObject *userData) const
{
  for (int i=m_fileLoaders.size()-1; i>=0; --i)
  {
    const iFileLoader *loader = m_fileLoaders[i];
    if (loader->CanLoad(file, locator))
    {
      iObject *obj = loader->Load(file, locator, userData);
      return obj;

    }
  }
  return 0;
}

const vkClass *vkResourceManager::EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData) const
{
  for (int i = m_fileLoaders.size() - 1; i >= 0; --i)
  {
    const iFileLoader *loader = m_fileLoaders[i];
    if (loader->CanLoad(file, locator))
    {
      return loader->EvalClass(file, locator, userData);
    }
  }
  return 0;
}


iObject *vkResourceManager::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  for (int i = m_xmlLoaders.size() - 1; i >= 0; --i)
  {
    const iXMLLoader *loader = m_xmlLoaders[i];
    if (loader->CanLoad(element, locator))
    {
      iObject *obj = loader->Load(element, locator, userData);
      return obj;
    }
  }
  return 0;
}

const vkClass *vkResourceManager::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  for (int i = m_xmlLoaders.size() - 1; i >= 0; --i)
  {
    const iXMLLoader *loader = m_xmlLoaders[i];
    if (loader->CanLoad(element, locator))
    {
      return loader->EvalClass(element, locator, userData);
    }
  }
  return 0;
}


iObject *vkResourceManager::Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData) const
{
  for (int i = m_assetLoaders.size() - 1; i >= 0; --i)
  {
    const iAssetLoader *loader = m_assetLoaders[i];
    if (loader->CanLoad(typeID, locator))
    {
      iObject *obj = loader->Load(inputStream, locator, userData);
      return obj;
    }
  }
  return 0;
}

const vkClass *vkResourceManager::EvalClass(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData) const
{
  for (int i = m_assetLoaders.size() - 1; i >= 0; --i)
  {
    const iAssetLoader *loader = m_assetLoaders[i];
    if (loader->CanLoad(typeID, locator))
    {
      return loader->EvalClass(inputStream, locator, userData);
    }
  }
  return 0;
}

iObject *vkResourceManager::Get(const vkResourceLocator &resourceLocator) const
{
  std::map<vkResourceLocator, iObject*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }
  return 0;
}

vkResourceLocator vkResourceManager::GetLocator(iObject *object) const
{
  std::map<iObject*, vkResourceLocator>::const_iterator it = m_resources.find(object);
  if (it != m_resources.end())
  {
    return it->second;
  }

  return vkResourceLocator();
}



iObject *vkResourceManager::GetOrLoad(const vkResourceLocator &resourceLocator, iObject *userData)
{
  std::map<vkResourceLocator, iObject*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }

  iObject *object = Load(resourceLocator, userData);
  if (!object)
  {
    return 0;
  }

  m_objects[resourceLocator] = object;
  m_resources[object] = resourceLocator;

  return object;
}



bool vkResourceManager::RegisterObject(const vkResourceLocator &resourceLocator, iObject *object)
{
  if (!object)
  {
    return false;
  }

  std::map<vkResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return false;
  }

  object->AddRef();
  m_objects[resourceLocator] = object;
  m_resources[object] = resourceLocator;
  return true;
}


void vkResourceManager::DeregisterObject(const vkResourceLocator &resourceLocator)
{
  std::map<vkResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    iObject *object = it->second;
    if (object)
    {
      std::map<iObject*, vkResourceLocator>::iterator rit = m_resources.find(object);
      if (rit != m_resources.end())
      {
        m_resources.erase(rit);
      }


      object->Release();
    }
    m_objects.erase(it);
  }
}

void vkResourceManager::DeregisterObject(iObject *object)
{
  if (!object)
  {
    return;
  }

  std::map<iObject*, vkResourceLocator>::iterator rit = m_resources.find(object);
  if (rit != m_resources.end())
  {
    const vkResourceLocator &resourceLocator = rit->second;
    std::map<vkResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
    if (it != m_objects.end())
    {
      m_objects.erase(it);
    }
    m_resources.erase(rit);

    object->Release();
  }
}


