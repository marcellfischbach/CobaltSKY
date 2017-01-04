
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/core/vkclassregistry.hh>
#include <Valkyrie/core/vkvfs.hh>





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


void vkResourceManager::RegisterLoader(IFileLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_fileLoaders.push_back(loader);
  }
}

void vkResourceManager::RegisterLoader(IXMLLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_xmlLoaders.push_back(loader);
  }
}

void vkResourceManager::RegisterLoader(IAssetLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_assetLoaders.push_back(loader);
  }
}

IObject *vkResourceManager::Load(const vkResourceLocator &locator, IObject *userData) const
{
  IFile *file = vkVFS::Get()->Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  IObject *object = Load(file, locator, userData);
  file->Release();

  return object;
}

const vkClass *vkResourceManager::EvalClass(const vkResourceLocator &locator, IObject *userData) const
{
  IFile *file = vkVFS::Get()->Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  const vkClass *cls = EvalClass(file, locator, userData);
  file->Release();

  return cls;
}

IObject *vkResourceManager::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  for (int i=m_fileLoaders.size()-1; i>=0; --i)
  {
    const IFileLoader *loader = m_fileLoaders[i];
    if (loader->CanLoad(file, locator))
    {
      IObject *obj = loader->Load(file, locator, userData);
      return obj;

    }
  }
  return 0;
}

const vkClass *vkResourceManager::EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  for (int i = m_fileLoaders.size() - 1; i >= 0; --i)
  {
    const IFileLoader *loader = m_fileLoaders[i];
    if (loader->CanLoad(file, locator))
    {
      return loader->EvalClass(file, locator, userData);
    }
  }
  return 0;
}


IObject *vkResourceManager::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  for (int i = m_xmlLoaders.size() - 1; i >= 0; --i)
  {
    const IXMLLoader *loader = m_xmlLoaders[i];
    if (loader->CanLoad(element, locator))
    {
      IObject *obj = loader->Load(element, locator, userData);
      return obj;
    }
  }
  return 0;
}

const vkClass *vkResourceManager::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  for (int i = m_xmlLoaders.size() - 1; i >= 0; --i)
  {
    const IXMLLoader *loader = m_xmlLoaders[i];
    if (loader->CanLoad(element, locator))
    {
      return loader->EvalClass(element, locator, userData);
    }
  }
  return 0;
}


IObject *vkResourceManager::Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  for (int i = m_assetLoaders.size() - 1; i >= 0; --i)
  {
    const IAssetLoader *loader = m_assetLoaders[i];
    if (loader->CanLoad(typeID, locator))
    {
      IObject *obj = loader->Load(inputStream, locator, userData);
      return obj;
    }
  }
  return 0;
}

const vkClass *vkResourceManager::EvalClass(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  for (int i = m_assetLoaders.size() - 1; i >= 0; --i)
  {
    const IAssetLoader *loader = m_assetLoaders[i];
    if (loader->CanLoad(typeID, locator))
    {
      return loader->EvalClass(inputStream, locator, userData);
    }
  }
  return 0;
}

IObject *vkResourceManager::Get(const vkResourceLocator &resourceLocator) const
{
  std::map<vkResourceLocator, IObject*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }
  return 0;
}

vkResourceLocator vkResourceManager::Get(IObject *object) const
{
  std::map<IObject*, vkResourceLocator>::const_iterator it = m_resources.find(object);
  if (it != m_resources.end())
  {
    return it->second;
  }

  return vkResourceLocator();
}



IObject *vkResourceManager::GetOrLoad(const vkResourceLocator &resourceLocator, IObject *userData)
{
  std::map<vkResourceLocator, IObject*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }

  IObject *object = Load(resourceLocator, userData);
  if (!object)
  {
    return 0;
  }

  m_objects[resourceLocator] = object;
  m_resources[object] = resourceLocator;

  return object;
}



bool vkResourceManager::RegisterObject(const vkResourceLocator &resourceLocator, IObject *object)
{
  if (!object)
  {
    return false;
  }

  std::map<vkResourceLocator, IObject*>::iterator it = m_objects.find(resourceLocator);
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
  std::map<vkResourceLocator, IObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    IObject *object = it->second;
    if (object)
    {
      std::map<IObject*, vkResourceLocator>::iterator rit = m_resources.find(object);
      if (rit != m_resources.end())
      {
        m_resources.erase(rit);
      }


      object->Release();
    }
    m_objects.erase(it);
  }
}

void vkResourceManager::DeregisterObject(IObject *object)
{
  if (!object)
  {
    return;
  }

  std::map<IObject*, vkResourceLocator>::iterator rit = m_resources.find(object);
  if (rit != m_resources.end())
  {
    const vkResourceLocator &resourceLocator = rit->second;
    std::map<vkResourceLocator, IObject*>::iterator it = m_objects.find(resourceLocator);
    if (it != m_objects.end())
    {
      m_objects.erase(it);
    }
    m_resources.erase(rit);

    object->Release();
  }
}


