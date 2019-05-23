
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/resource/cscsffileloader.hh>
#include <cobalt/core/csvfs.hh>
#include <csrefl/classregistry.hh>
#include <iostream>


cs::ResourceManager *cs::ResourceManager::s_instance = nullptr;


cs::ResourceManager *cs::ResourceManager::Get()
{
  if (!cs::ResourceManager::s_instance)
  {
    cs::ResourceManager::s_instance = new cs::ResourceManager();
  }
  return cs::ResourceManager::s_instance;
}

void cs::ResourceManager::Register(cs::ResourceManager *resourceManager)
{
  if (cs::ResourceManager::s_instance)
  {
    // TODO: move all resources from the current resource manager to the new
    delete cs::ResourceManager::s_instance;
  }
  cs::ResourceManager::s_instance = resourceManager;
}


cs::ResourceManager::ResourceManager()
{
  RegisterLoader(new cs::CSFFileLoader());
  RegisterLoader(new cs::AssetCSFLoader());
}


void cs::ResourceManager::RegisterLoader(cs::iFileLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_fileLoaders.push_back(loader);
  }
}

void cs::ResourceManager::RegisterLoader(cs::iCSFLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_csfLoaders.push_back(loader);
  }
}
void cs::ResourceManager::RegisterLoader(cs::iAssetLoader *loader)
{
  if (loader)
  {
    loader->AddRef();
    m_assetLoaders.push_back(loader);
  }
}

cs::iFile *cs::ResourceManager::Open(const cs::ResourceLocator &locator) const
{
  if (locator.GetResourceEntry().empty())
  {
    return cs::VFS::Get()->Open(locator.GetResourceFile());
  }
  else
  {
    return cs::VFS::Get()->Open(locator.GetResourceFile(), locator.GetResourceEntry());
  }
}

void cs::ResourceManager::Reload(const cs::ResourceLocator &locator)
{
  auto it = m_objects.find(locator);
  if (it == m_objects.end())
  {
    return;
  }

  std::string path = cs::VFS::Get()->GetAbsolutePath(locator);
  std::cout << "Reload from: " << path << "\n";

  cs::ResourceWrapper *currentWrapper = it->second;
  cs::ResourceWrapper *newWrapper = Load(locator);
  if (newWrapper && currentWrapper)
  {
    currentWrapper->Set(newWrapper->Get());
  }
  std::flush(std::cout);
}

cs::ResourceWrapper *cs::ResourceManager::Load(const cs::ResourceLocator &locator)
{
  cs::iFile *file = Open(locator);
  if (!file)
  {
    return nullptr;
  }

  cs::ResourceWrapper *object = Load(file, locator);
  if (object)
  {
    object->SetLocator(locator);
  }

  file->Release();
  return object;
}

const cs::Class *cs::ResourceManager::EvalClass(const cs::ResourceLocator &locator) const
{
  cs::iFile *file = Open(locator);
  if (!file)
  {
    return nullptr;
  }

  const cs::Class *cls = EvalClass(file, locator);
  file->Release();

  return cls;
}

cs::ResourceWrapper *cs::ResourceManager::Load(cs::iFile *file, const cs::ResourceLocator &locator)
{
  for (int i = static_cast<int>(m_fileLoaders.size()) - 1; i >= 0; --i)
  {
    const cs::iFileLoader *loader = m_fileLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(file, locator))
    {
      cs::ResourceWrapper *obj = loader->Load(file, locator);
      return obj;

    }
  }
  return nullptr;
}

const cs::Class *cs::ResourceManager::EvalClass(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  for (int i = static_cast<int>(m_fileLoaders.size()) - 1; i >= 0; --i)
  {
    const cs::iFileLoader *loader = m_fileLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(file, locator))
    {
      return loader->EvalClass(file, locator);
    }
  }
  return nullptr;
}


cs::ResourceWrapper *cs::ResourceManager::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData)
{
  for (int i = static_cast<int>(m_csfLoaders.size()) - 1; i >= 0; --i)
  {
    const cs::iCSFLoader *loader = m_csfLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(entry, locator))
    {
      cs::ResourceWrapper *obj = loader->Load(entry, locator, userData);
      return obj;
    }
  }
  return nullptr;
}

const cs::Class *cs::ResourceManager::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  for (int i = static_cast<int>(m_csfLoaders.size()) - 1; i >= 0; --i)
  {
    const cs::iCSFLoader *loader = m_csfLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(entry, locator))
    {
      return loader->EvalClass(entry, locator, userData);
    }
  }
  return nullptr;
}


cs::ResourceWrapper *cs::ResourceManager::Load(const cs::file::Blob *blob, const cs::ResourceLocator &locator, cs::iObject *userData)
{
  cs::AssetInputStream stream (static_cast<const csUInt8*>(blob->GetBuffer()), blob->GetSize());
  return Load(blob->GetType(), stream, locator, userData);
}
const cs::Class *cs::ResourceManager::EvalClass(const cs::file::Blob *blob, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::AssetInputStream stream (static_cast<const csUInt8*>(blob->GetBuffer()), blob->GetSize());
  return EvalClass(blob->GetType(), stream, locator, userData);
}


cs::ResourceWrapper *cs::ResourceManager::Load(const std::string &typeID, cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData)
{
  for (int i = static_cast<int>(m_assetLoaders.size()) - 1; i >= 0; --i)
  {
    const cs::iAssetLoader *loader = m_assetLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(typeID, locator))
    {
      cs::ResourceWrapper *obj = loader->Load(inputStream, locator, userData);
      return obj;
    }
  }
  return nullptr;
}

const cs::Class *cs::ResourceManager::EvalClass(const std::string &typeID, cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  for (int i = static_cast<int>(m_assetLoaders.size()) - 1; i >= 0; --i)
  {
    const cs::iAssetLoader *loader = m_assetLoaders[static_cast<size_t>(i)];
    if (loader->CanLoad(typeID, locator))
    {
      return loader->EvalClass(inputStream, locator, userData);
    }
  }
  return nullptr;
}

cs::ResourceWrapper *cs::ResourceManager::Get(const cs::ResourceLocator &resourceLocator) const
{
  std::map<cs::ResourceLocator, cs::ResourceWrapper*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }
  return nullptr;
}



cs::ResourceWrapper *cs::ResourceManager::GetOrLoad(const cs::ResourceLocator &resourceLocator)
{
  std::map<cs::ResourceLocator, cs::ResourceWrapper*>::const_iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return it->second;
  }

  cs::ResourceWrapper *object = Load(resourceLocator);
  if (!object)
  {
    return nullptr;
  }

  RegisterObject(resourceLocator, object);

  return object;
}

cs::ResourceWrapper *cs::ResourceManager::Aquire(const cs::ResourceLocator &resourceLocator, cs::eResourceLoadingMode mode)
{
  cs::ResourceWrapper* res = nullptr;
  switch (mode)
  {
  case cs::eRLM_Shared:
    res = GetOrLoad(resourceLocator);
    break;
  case cs::eRLM_Instance:
    res = Load(resourceLocator);
    break;
  case cs::eRLM_Inline:
    break;
  }
  return res;
}



bool cs::ResourceManager::RegisterObject(const cs::ResourceLocator &resourceLocator, cs::ResourceWrapper *object)
{
  if (!object)
  {
    return false;
  }

  std::map<cs::ResourceLocator, cs::ResourceWrapper*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    return false;
  }

  m_objects[resourceLocator] = object;


  return true;
}


void cs::ResourceManager::UnregisterObject(const cs::ResourceLocator &resourceLocator)
{
  std::map<cs::ResourceLocator, cs::ResourceWrapper*>::iterator it = m_objects.find(resourceLocator);
  if (it != m_objects.end())
  {
    cs::ResourceWrapper *object = it->second;
    UnregisterObject(object);
  }

}

void cs::ResourceManager::UnregisterObject(cs::ResourceWrapper *object)
{
  if (object)
  {

    std::vector<cs::ResourceLocator> toDelete;
    for (auto it : m_objects)
    {
      if (it.second == object)
      {
        toDelete.push_back(it.first);
      }
    }

    for (cs::ResourceLocator loc : toDelete)
    {
      m_objects.erase(loc);
    }
  }

}



void cs::ResourceManager::RenameResource(const cs::ResourceLocator &from, const cs::ResourceLocator &to)
{
  auto it = m_objects.find(from);
  if (it == m_objects.end())
  {
    return;
  }
  cs::ResourceWrapper *object = it->second;

  m_objects.erase(it);
  m_objects[to] = object;
}

