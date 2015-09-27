
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/VFS.hh>

vkResourceManager *vkResourceManager::Get()
{
  static vkResourceManager static_manager;
  return &static_manager;
}


vkResourceManager::vkResourceManager()
{
  RegisterLoader(new vkXMLFileLoader());
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

IObject *vkResourceManager::Load(const vkResourceLocator &locator) const
{
  IFile *file = vkVFS::Get()->Open(locator.GetResourceFile());
  if (!file)
  {
    return 0;
  }

  IObject *object = Load(file, locator);
  file->Release();

  return object;
}

IObject *vkResourceManager::Load(IFile *file, const vkResourceLocator &locator) const
{
  for (auto loader : m_fileLoaders)
  {
    if (loader->CanLoad(file, locator))
    {
      IObject *obj = loader->Load(file, locator);
      return obj;

    }
  }
  return 0;
}

IObject *vkResourceManager::Load(TiXmlElement *element, const vkResourceLocator &locator) const
{
  for (auto loader : m_xmlLoaders)
  {
    if (loader->CanLoad(element, locator))
    {
      IObject *obj = loader->Load(element, locator);
      return obj;
    }
  }
  return 0;
}




vkResourceLocator::vkResourceLocator(const vkString &resourceFile, const vkString &resourceName)
  : m_resourceFile(resourceFile)
  , m_resourceName(resourceName)
{

}

const vkString &vkResourceLocator::GetResourceFile() const
{
  return m_resourceFile;
}

const vkString &vkResourceLocator::GetResourceName() const
{
  return m_resourceName;
}

bool vkResourceLocator::operator<(const vkResourceLocator &o) const
{
  if (m_resourceFile < o.m_resourceFile) return true;
  if (m_resourceFile > o.m_resourceFile) return false;
  if (m_resourceName < o.m_resourceName) return true;
  return false;
}


vkXMLFileLoader::vkXMLFileLoader()
  : IFileLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkXMLFileLoader::~vkXMLFileLoader()
{
}

bool vkXMLFileLoader::CanLoad(IFile *file, const vkResourceLocator &locator) const
{
  return file->GetExtension() == vkString("xml");
}


IObject *vkXMLFileLoader::Load(IFile *file, const vkResourceLocator &locator) const
{
  if (file->GetExtension() == vkString("xml"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  long currentPos = file->Tell();

  // get the length of the file
  file->Seek(eSP_Set, 0);
  long length = file->Tell();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, currentPos);


  TiXmlDocument document;
  document.Parse(buffer);
  delete[] buffer;
  if (document.Error())
  {
    return 0;
  }

  return vkResourceManager::Get()->Load(document.RootElement(), locator);
}