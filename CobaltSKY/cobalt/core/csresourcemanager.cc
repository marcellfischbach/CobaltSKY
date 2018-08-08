
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/resource/cscsffileloader.hh>
#include <cobalt/core/csvfs.hh>


csResourceManager *csResourceManager::s_instance = 0;


csResourceManager *csResourceManager::Get()
{
  if (!csResourceManager::s_instance)
  {
    csResourceManager::s_instance = new csResourceManager();
  }
  return csResourceManager::s_instance;
}

void csResourceManager::Register(csResourceManager *resourceManager)
{
  if (csResourceManager::s_instance)
  {
    // TODO: move all resources from the current resource manager to the new
    delete csResourceManager::s_instance;
  }
  csResourceManager::s_instance = resourceManager;
}


csResourceManager::csResourceManager()
{
  RegisterLoader(new csCSFFileLoader());
  RegisterLoader(new csAssetCSFLoader());
}


void csResourceManager::RegisterLoader(iFileLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_fileLoaders.push_back(loader);
  }
}

void csResourceManager::RegisterLoader(iCSFLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_csfLoaders.push_back(loader);
  }
}
void csResourceManager::RegisterLoader(iAssetLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_assetLoaders.push_back(loader);
  }
}

iFile *csResourceManager::Open(const csResourceLocator &locator) const
{
  if (locator.GetResourceEntry().empty())
  {
    return csVFS::Get()->Open(locator.GetResourceFile());
  }
  else
  {
    return csVFS::Get()->Open(locator.GetResourceFile(), locator.GetResourceEntry());
  }
}

iObject *csResourceManager::Load(const csResourceLocator &locator, iObject *userData)
{
  iFile *file = Open(locator);
  if (!file)
  {
    return 0;
  }

  iObject *object = Load(file, locator, userData);
  file->Release();
  RegisterResource(object, locator);
  return object;
}

const csClass *csResourceManager::EvalClass(const csResourceLocator &locator, iObject *userData) const
{
  iFile *file = Open(locator);
  if (!file)
  {
    return 0;
  }

  const csClass *cls = EvalClass(file, locator, userData);
  file->Release();

  return cls;
}

iObject *csResourceManager::Load(iFile *file, const csResourceLocator &locator, iObject *userData)
{
  for (int i = (int)m_fileLoaders.size() - 1; i >= 0; --i)
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

const csClass *csResourceManager::EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  for (int i = (int)m_fileLoaders.size() - 1; i >= 0; --i)
  {
    const iFileLoader *loader = m_fileLoaders[i];
    if (loader->CanLoad(file, locator))
    {
      return loader->EvalClass(file, locator, userData);
    }
  }
  return 0;
}


iObject *csResourceManager::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData)
{
  for (int i = (int)m_csfLoaders.size() - 1; i >= 0; --i)
  {
    const iCSFLoader *loader = m_csfLoaders[i];
    if (loader->CanLoad(entry, locator))
    {
      iObject *obj = loader->Load(entry, locator, userData);
      RegisterResource(obj, locator);
      return obj;
    }
  }
  return 0;
}

const csClass *csResourceManager::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  for (int i = (int)m_csfLoaders.size() - 1; i >= 0; --i)
  {
    const iCSFLoader *loader = m_csfLoaders[i];
    if (loader->CanLoad(entry, locator))
    {
      return loader->EvalClass(entry, locator, userData);
    }
  }
  return 0;
}


iObject *csResourceManager::Load(const csfBlob *blob, const csResourceLocator &locator, iObject *userData)
{
  csAssetInputStream stream (static_cast<const csUInt8*>(blob->GetBuffer()), blob->GetSize());
  return Load(blob->GetType(), stream, locator, userData);
}
const csClass *csResourceManager::EvalClass(const csfBlob *blob, const csResourceLocator &locator, iObject *userData) const
{
  csAssetInputStream stream (static_cast<const csUInt8*>(blob->GetBuffer()), blob->GetSize());
  return EvalClass(blob->GetType(), stream, locator, userData);
}


iObject *csResourceManager::Load(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData)
{
  for (int i = (int)m_assetLoaders.size() - 1; i >= 0; --i)
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

const csClass *csResourceManager::EvalClass(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  for (int i = (int)m_assetLoaders.size() - 1; i >= 0; --i)
  {
    const iAssetLoader *loader = m_assetLoaders[i];
    if (loader->CanLoad(typeID, locator))
    {
      return loader->EvalClass(inputStream, locator, userData);
    }
  }
  return 0;
}

iObject *csResourceManager::Get(const csResourceLocator &resourceLocator) const
{
  std::map<csResourceLocator, iObject*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }
  return 0;
}

csResourceLocator csResourceManager::GetLocator(iObject *object) const
{
  std::map<iObject*, csResourceLocator>::const_iterator it = m_resources.find(object);
  if (it != m_resources.end())
  {
    return it->second;
  }

  return csResourceLocator();
}



iObject *csResourceManager::GetOrLoad(const csResourceLocator &resourceLocator, iObject *userData)
{
  std::map<csResourceLocator, iObject*>::const_iterator it = m_objects.find(resourceLocator);
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

iObject *csResourceManager::Aquire(const csResourceLocator &resourceLocator, iObject *userData, csResourceLoadingMode mode)
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




bool csResourceManager::RegisterObject(const csResourceLocator &resourceLocator, iObject *object)
{
  RegisterResource(object, resourceLocator);

  if (!object)
  {
    return false;
  }

  std::map<csResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return false;
  }

  m_objects[resourceLocator] = object;


  return true;
}


void csResourceManager::UnregisterObject(const csResourceLocator &resourceLocator)
{
  std::map<csResourceLocator, iObject*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    iObject *object = it->second;
    UnregisterObject(object);
  }

}

void csResourceManager::UnregisterObject(iObject *object)
{
  if (object)
  {

    std::vector<csResourceLocator> toDelete;
    for (auto it : m_objects)
    {
      if (it.second == object)
      {
        toDelete.push_back(it.first);
      }
    }

    for (csResourceLocator loc : toDelete)
    {
      m_objects.erase(loc);
    }
  }

  UnregisterResource(object);
}


void csResourceManager::RegisterResource(iObject *object, const csResourceLocator &locator)
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

void csResourceManager::UnregisterResource(iObject *object)
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



void csResourceManager::RenameResource(const csResourceLocator &from, const csResourceLocator &to)
{
  auto it = m_objects.find(from);
  if (it == m_objects.end())
  {
    return;
  }
  iObject *object = it->second;

  m_objects.erase(it);
  m_objects[to] = object;
  m_resources[object] = to;
}

