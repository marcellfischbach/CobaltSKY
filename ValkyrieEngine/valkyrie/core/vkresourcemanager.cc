
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <valkyrie/core/vkvfs.hh>


vkResourceManager *vkResourceManager::s_instance = 0;


vkResourceManager *vkResourceManager::Get()
{
  if (!vkResourceManager::s_instance)
  {
    vkResourceManager::s_instance = new vkResourceManager();
  }
  return vkResourceManager::s_instance;
}

void vkResourceManager::Register(vkResourceManager *resourceManager)
{
  if (vkResourceManager::s_instance)
  {
    // TODO: move all resources from the current resource manager to the new
    delete vkResourceManager::s_instance;
  }
  vkResourceManager::s_instance = resourceManager;
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

iFile *vkResourceManager::Open(const vkResourceLocator &locator) const
{
  if (locator.GetResourceEntry().empty())
  {
    return vkVFS::Get()->Open(locator.GetResourceFile());
  }
  else
  {
    return vkVFS::Get()->Open(locator.GetResourceFile(), locator.GetResourceEntry());
  }
}

iObject *vkResourceManager::Load(const vkResourceLocator &locator, iObject *userData)
{
  iFile *file = Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  iObject *object = Load(file, locator, userData);
  file->Release();
  RegisterResource(object, locator);
  return object;
}

const vkClass *vkResourceManager::EvalClass(const vkResourceLocator &locator, iObject *userData) const
{
  iFile *file = Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  const vkClass *cls = EvalClass(file, locator, userData);
  file->Release();

  return cls;
}

iObject *vkResourceManager::Load(iFile *file, const vkResourceLocator &locator, iObject *userData)
{
  for (int i = m_fileLoaders.size() - 1; i >= 0; --i)
  {
    const iFileLoader *loader = m_fileLoaders[i];
    if (loader->CanLoad(file, locator))
    {
      iObject *obj = loader->Load(file, locator, userData);
      RegisterResource(obj, locator);
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


iObject *vkResourceManager::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData)
{
  for (int i = m_xmlLoaders.size() - 1; i >= 0; --i)
  {
    const iXMLLoader *loader = m_xmlLoaders[i];
    if (loader->CanLoad(element, locator))
    {
      iObject *obj = loader->Load(element, locator, userData);
      RegisterResource(obj, locator);
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


iObject *vkResourceManager::Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData)
{
  for (int i = m_assetLoaders.size() - 1; i >= 0; --i)
  {
    const iAssetLoader *loader = m_assetLoaders[i];
    if (loader->CanLoad(typeID, locator))
    {
      iObject *obj = loader->Load(inputStream, locator, userData);
      RegisterResource(obj, locator);
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

  RegisterObject(resourceLocator, object);

  return object;
}

iObject *vkResourceManager::Aquire(const vkResourceLocator &resourceLocator, iObject *userData, vkResourceLoadingMode mode)
{
  iObject* res = 0;
  switch (mode)
  {
  case eRLM_Shared:
    res = GetOrLoad(resourceLocator, userData);
    break;
  case eRLM_Instance:
    res = Load(resourceLocator, userData);
    break;
  }
  return res;
}




bool vkResourceManager::RegisterObject(const vkResourceLocator &resourceLocator, iObject *object)
{
  RegisterResource(object, resourceLocator);

  if (!object)
  {
    return false;
  }

  std::map<vkResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return false;
  }

  m_objects[resourceLocator] = object;


  return true;
}


void vkResourceManager::UnregisterObject(const vkResourceLocator &resourceLocator)
{
  std::map<vkResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    iObject *object = it->second;
    UnregisterObject(object);
  }

}

void vkResourceManager::UnregisterObject(iObject *object)
{
  if (object)
  {

    std::vector<vkResourceLocator> toDelete;
    for (auto it : m_objects)
    {
      if (it.second == object)
      {
        toDelete.push_back(it.first);
      }
    }

    for (vkResourceLocator loc : toDelete)
    {
      m_objects.erase(loc);
    }
  }

  UnregisterResource(object);
}


void vkResourceManager::RegisterResource(iObject *object, const vkResourceLocator &locator)
{
  if (!object)
  {
    return;
  }

  auto it = m_resources.find(object);
  if (it != m_resources.end())
  {
    // this object is already registered and the resource manager hold a reference
    return;
  }

  object->AddRef();
  m_resources[object] = locator;
}

void vkResourceManager::UnregisterResource(iObject *object)
{
  if (!object)
  {
    return;
  }

  auto it = m_resources.find(object);
  if (it == m_resources.end())
  {
    // this object is not registered and the resource manager don't hold a reference
    return;
  }

  m_resources.erase(it);
  object->Release();
}



