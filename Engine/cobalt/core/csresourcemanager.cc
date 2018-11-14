
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/resource/cscsffileloader.hh>
#include <cobalt/core/csvfs.hh>
#include <iostream>


csResourceManager *csResourceManager::s_instance = nullptr;


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

void csResourceManager::Reload(const csResourceLocator &locator)
{
  auto it = m_objects.find(locator);
  if (it == m_objects.end())
  {
    return;
  }

  std::string path = csVFS::Get()->GetAbsolutePath(locator);
  std::cout << "Reload from: " << path << "\n";

  csResourceWrapper *currentWrapper = it->second;
  csResourceWrapper *newWrapper = Load(locator);
  if (newWrapper && currentWrapper)
  {
    currentWrapper->Set(newWrapper->Get());
  }
  std::flush(std::cout);
}

csResourceWrapper *csResourceManager::Load(const csResourceLocator &locator)
{
  iFile *file = Open(locator);
  if (!file)
  {
    return nullptr;
  }

  csResourceWrapper *object = Load(file, locator);
  if (object)
  {
    object->SetLocator(locator);
  }

  file->Release();
  return object;
}

const csClass *csResourceManager::EvalClass(const csResourceLocator &locator) const
{
  iFile *file = Open(locator);
  if (!file)
  {
    return nullptr;
  }

  const csClass *cls = EvalClass(file, locator);
  file->Release();

  return cls;
}

csResourceWrapper *csResourceManager::Load(iFile *file, const csResourceLocator &locator)
{
  for (int i = static_cast<int>(m_fileLoaders.size()) - 1; i >= 0; --i)
  {
    const iFileLoader *loader = m_fileLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(file, locator))
    {
      csResourceWrapper *obj = loader->Load(file, locator);
      return obj;

    }
  }
  return nullptr;
}

const csClass *csResourceManager::EvalClass(iFile *file, const csResourceLocator &locator) const
{
  for (int i = static_cast<int>(m_fileLoaders.size()) - 1; i >= 0; --i)
  {
    const iFileLoader *loader = m_fileLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(file, locator))
    {
      return loader->EvalClass(file, locator);
    }
  }
  return nullptr;
}


csResourceWrapper *csResourceManager::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData)
{
  for (int i = static_cast<int>(m_csfLoaders.size()) - 1; i >= 0; --i)
  {
    const iCSFLoader *loader = m_csfLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(entry, locator))
    {
      csResourceWrapper *obj = loader->Load(entry, locator, userData);
      return obj;
    }
  }
  return nullptr;
}

const csClass *csResourceManager::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  for (int i = static_cast<int>(m_csfLoaders.size()) - 1; i >= 0; --i)
  {
    const iCSFLoader *loader = m_csfLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(entry, locator))
    {
      return loader->EvalClass(entry, locator, userData);
    }
  }
  return nullptr;
}


csResourceWrapper *csResourceManager::Load(const csfBlob *blob, const csResourceLocator &locator, iObject *userData)
{
  csAssetInputStream stream (static_cast<const csUInt8*>(blob->GetBuffer()), blob->GetSize());
  return Load(blob->GetType(), stream, locator, userData);
}
const csClass *csResourceManager::EvalClass(const csfBlob *blob, const csResourceLocator &locator, iObject *userData) const
{
  csAssetInputStream stream (static_cast<const csUInt8*>(blob->GetBuffer()), blob->GetSize());
  return EvalClass(blob->GetType(), stream, locator, userData);
}


csResourceWrapper *csResourceManager::Load(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData)
{
  for (int i = static_cast<int>(m_assetLoaders.size()) - 1; i >= 0; --i)
  {
    const iAssetLoader *loader = m_assetLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(typeID, locator))
    {
      csResourceWrapper *obj = loader->Load(inputStream, locator, userData);
      return obj;
    }
  }
  return nullptr;
}

const csClass *csResourceManager::EvalClass(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  for (int i = static_cast<int>(m_assetLoaders.size()) - 1; i >= 0; --i)
  {
    const iAssetLoader *loader = m_assetLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(typeID, locator))
    {
      return loader->EvalClass(inputStream, locator, userData);
    }
  }
  return nullptr;
}

csResourceWrapper *csResourceManager::Get(const csResourceLocator &resourceLocator) const
{
  std::map<csResourceLocator, csResourceWrapper*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }
  return nullptr;
}



csResourceWrapper *csResourceManager::GetOrLoad(const csResourceLocator &resourceLocator)
{
  std::map<csResourceLocator, csResourceWrapper*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }

  csResourceWrapper *object = Load(resourceLocator);
  if (!object)
  {
    return nullptr;
  }

  RegisterObject(resourceLocator, object);

  return object;
}

csResourceWrapper *csResourceManager::Aquire(const csResourceLocator &resourceLocator, csResourceLoadingMode mode)
{
  csResourceWrapper* res = nullptr;
  switch (mode)
  {
  case eRLM_Shared:
    res = GetOrLoad(resourceLocator);
    break;
  case eRLM_Instance:
    res = Load(resourceLocator);
    break;
  case eRLM_Inline:
    break;
  }
  return res;
}



bool csResourceManager::RegisterObject(const csResourceLocator &resourceLocator, csResourceWrapper *object)
{
  if (!object)
  {
    return false;
  }

  std::map<csResourceLocator, csResourceWrapper*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return false;
  }

  m_objects[resourceLocator] = object;


  return true;
}


void csResourceManager::UnregisterObject(const csResourceLocator &resourceLocator)
{
  std::map<csResourceLocator, csResourceWrapper*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    csResourceWrapper *object = it->second;
    UnregisterObject(object);
  }

}

void csResourceManager::UnregisterObject(csResourceWrapper *object)
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

}



void csResourceManager::RenameResource(const csResourceLocator &from, const csResourceLocator &to)
{
  auto it = m_objects.find(from);
  if (it == m_objects.end())
  {
    return;
  }
  csResourceWrapper *object = it->second;

  m_objects.erase(it);
  m_objects[to] = object;
}

