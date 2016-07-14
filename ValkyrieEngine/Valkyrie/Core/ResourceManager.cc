
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Core/VFS.hh>

vkResourceManager *vkResourceManager::Get()
{
  static vkResourceManager static_manager;
  return &static_manager;
}


vkResourceManager::vkResourceManager()
{
  RegisterLoader(new vkXMLFileLoader());
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

IObject *vkResourceManager::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  for (auto loader : m_fileLoaders)
  {
    if (loader->CanLoad(file, locator))
    {
      IObject *obj = loader->Load(file, locator, userData);
      return obj;

    }
  }
  return 0;
}

IObject *vkResourceManager::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  for (auto loader : m_xmlLoaders)
  {
    if (loader->CanLoad(element, locator))
    {
      IObject *obj = loader->Load(element, locator, userData);
      return obj;
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
      object->Release();
    }
    m_objects.erase(it);
  }
}


vkResourceLocator::vkResourceLocator(const vkString &resourceFile, const vkString &resourceName)
  : m_resourceFile(resourceFile)
  , m_resourceName(resourceName)
{
  if (resourceName.length() == 0)
  {
    size_t idx = m_resourceFile.find(':');
    if (idx != std::string::npos)
    {
      m_resourceName = m_resourceFile.substr(idx + 1);
      m_resourceFile = m_resourceFile.substr(0, idx);
    }
  }
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


bool vkResourceLocator::operator==(const vkResourceLocator &o) const
{
  return m_resourceFile == o.m_resourceFile && m_resourceName == o.m_resourceName;
}

bool vkResourceLocator::IsValid() const
{
  return m_resourceFile.size() > 0;
}

vkAssetFileLoader::vkAssetFileLoader()
  : IFileLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkAssetFileLoader::~vkAssetFileLoader()
{

}

bool vkAssetFileLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  return file->GetExtension() == vkString("asset");
}


class AssetLoaderCache
{
public:
  static IAssetLoader *Get(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData)
  {
    for (size_t i = 0, in = s_loaders.size(); i < in; ++i)
    {
      if (s_loaders[i]->CanLoad(typeID, name, locator, userData))
      {
        return s_loaders[i];
      }
    }

    IAssetLoader *finalLoader = 0;
    std::vector<const vkClass*> allClasses = vkClassRegistry::Get()->GetAllClasses();
    for (const vkClass *clazz : allClasses)
    {
      if (clazz->IsInstanceOf <IAssetLoader>())
      {
        // check if this class is already within the list.
        bool alreadyInList = false;
        for (size_t i = 0, in = s_loaders.size(); i < in; ++i)
        {
          if (s_loaders[i]->GetClass() == clazz)
          {
            alreadyInList = true;
            break;
          }
        }

        if (alreadyInList)
        {
          continue;
        }


        IAssetLoader *assetLoader = clazz->CreateInstance<IAssetLoader>();
        if (!assetLoader)
        {
          // this can happen 
          // there are interfaces wihtin the list aswell
          continue;
        }
        s_loaders.push_back(assetLoader);
        if (!finalLoader && assetLoader->CanLoad(typeID, name, locator, userData))
        {
          finalLoader = assetLoader;
        }
      }
    }
    return finalLoader;
  }
private:
  AssetLoaderCache() {}
  static std::vector<IAssetLoader*> s_loaders;
};

std::vector<IAssetLoader*> AssetLoaderCache::s_loaders;

IObject *vkAssetFileLoader::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  char magicNumber[9];
  file->Read(magicNumber, 8);
  magicNumber[8] = '\0';
  if (vkString(magicNumber) != vkString("VALASSET"))
  {
    return 0;
  }

  vkUInt32 assetVersion;
  file->Read(&assetVersion, sizeof(vkUInt32));

  vkUInt16 numEntries;
  file->Read(&numEntries, sizeof(vkUInt16));

  for (vkUInt16 i = 0; i < numEntries; ++i)
  {
#pragma pack(1)
    struct Entry
    {
      char typeID[64];
      char name[64];
      vkUInt32 offset;
    } entry;

    file->Read(&entry, sizeof(Entry));

    if (locator.GetResourceName().length() == 0 || locator.GetResourceName() == vkString(entry.name))
    {
      // move pointer to the right position within the file
      file->Seek(eSP_Set, entry.offset);
#pragma pack(1)
      vkUInt32 length;
      file->Read(&length, sizeof(vkUInt32));

      IAssetLoader *assetLoader = AssetLoaderCache::Get(vkString(entry.typeID), vkString(entry.name), locator, userData);
      if (!assetLoader)
      {
        return 0;
      }

      vkUInt8 *buffer = new vkUInt8[length];
      file->Read(buffer, length);

      vkAssetInputStream is(buffer, (vkSize)length);
      IObject *obj = 0;

      obj = assetLoader->Load(is, locator, userData);
      delete[] buffer;
      // load the data
      return obj;
    }
  }

  return 0;
}

vkXMLFileLoader::vkXMLFileLoader()
  : IFileLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkXMLFileLoader::~vkXMLFileLoader()
{
}

bool vkXMLFileLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  return file->GetExtension() == vkString("xml");
}


IObject *vkXMLFileLoader::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  if (file->GetExtension() != vkString("xml"))
  {
    return 0;
  }

  // save the current position (we will reset this possition at the end)
  vkSize currentPos = file->Tell();

  // get the length of the file
  vkSize length = file->GetLength();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, (long)currentPos);


  TiXmlDocument document;
  document.Parse(buffer);
  delete[] buffer;
  if (document.Error())
  {
    printf("Unable to parse xml file: %s\n%s\n", 
           locator.GetResourceFile().c_str(), document.ErrorDesc());
    return 0;
  }

  return vkResourceManager::Get()->Load(document.RootElement(), locator);
}

vkBaseXMLLoader::vkBaseXMLLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkBaseXMLLoader::~vkBaseXMLLoader()
{

}

TiXmlElement *vkBaseXMLLoader::FindElement(TiXmlElement *root, const vkString &elementName, const vkString &name) const
{
  if (!root)
  {
    return 0;
  }

  vkString tagName(root->Value());
  if (tagName == elementName)
  {
    return root;
  }

  for (TiXmlElement *element = root->FirstChildElement(elementName.c_str());
  element;
    element = element->NextSiblingElement(elementName.c_str()))
  {
    if (name.length() == 0)
    {
      return element;
    }

    const char *c_attr = element->Attribute("name");
    if (!c_attr)
    {
      continue;
    }
    vkString thisName(c_attr);
    if (thisName == name)
    {
      return element;
    }
  }

  return 0;
}

TiXmlElement *vkBaseXMLLoader::FindElementByTagName(TiXmlElement *root, const vkString &elementName) const
{
  if (!root)
  {
    return 0;
  }

  vkString tagName(root->Value());
  if (tagName == elementName)
  {
    return root;
  }

  for (TiXmlElement *element = root->FirstChildElement();
       element;
       element = element->NextSiblingElement())
  {
    if (elementName.length() == 0 || vkString(element->Value ()) == elementName)
    {
      return element;
    }
  }

  return 0;
}


vkResourceLoadingMode vkBaseXMLLoader::GetResourceLoadingMode(TiXmlElement *element, vkResourceLoadingMode defaultMode, vkResourceLoadingMode alterInline) const
{
  vkResourceLoadingMode mode = defaultMode;
  if (element->Attribute("resourcemode"))
  {
    vkString modeString(element->Attribute("resourcemode"));
    if (modeString == vkString("shared"))
    {
      mode = eRLM_Shared;
    }
    else if (modeString == vkString("instance"))
    {
      mode = eRLM_Instance;
    }
    else if (modeString == vkString("inline"))
    {
      mode = alterInline;
    }
  }
  if (!element->GetText())
  {
    // no references should be inline
    mode = alterInline;
  }
  return mode;
}

bool vkBaseXMLLoader::LoadBool(const char *str) const
{
  return str == std::string("true");
}

float vkBaseXMLLoader::LoadFloat(const char *str) const
{
  return (float) atof(str);
}


vkVector2f vkBaseXMLLoader::LoadVector2f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';
#ifdef VK_WIN32
  char *ctx;
  char *sx = strtok_s(s, " ;,", &ctx);
  char *sy = strtok_s(0, " ;,", &ctx);
#else
  char *sx = strtok(s, " ;,");
  char *sy = strtok(0, " ;,");
#endif

  float x = (float)atof(sx);
  float y = (float)atof(sy);
  delete[] s;

  return vkVector2f(x, y);
}

vkVector3f vkBaseXMLLoader::LoadVector3f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef VK_WIN32
  char *ctx;
  char *sx = strtok_s(s, " ;,", &ctx);
  char *sy = strtok_s(0, " ;,", &ctx);
  char *sz = strtok_s(0, " ;,", &ctx);
#else
  char *sx = strtok(s, " ;,");
  char *sy = strtok(0, " ;,");
  char *sz = strtok(0, " ;,");
#endif

  float x = (float)atof(sx);
  float y = (float)atof(sy);
  float z = (float)atof(sz);
  delete[] s;

  return vkVector3f(x, y, z);
}

vkVector4f vkBaseXMLLoader::LoadVector4f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef VK_WIN32
  char *ctx;
  char *sx = strtok_s(s, " ;,", &ctx);
  char *sy = strtok_s(0, " ;,", &ctx);
  char *sz = strtok_s(0, " ;,", &ctx);
  char *sw = strtok_s(0, " ;,", &ctx);
#else
  char *sx = strtok(s, " ;,");
  char *sy = strtok(0, " ;,");
  char *sz = strtok(0, " ;,");
  char *sw = strtok(0, " ;,");
#endif

  float x = (float)atof(sx);
  float y = (float)atof(sy);
  float z = (float)atof(sz);
  float w = (float)atof(sw);
  delete[] s;

  return vkVector4f(x, y, z, w);
}

vkColor4f vkBaseXMLLoader::LoadColor4f(const char *str) const
{
  size_t l = strlen(str);
  char *s = new char[l + 1];
  memcpy(s, str, l);
  s[l] = '\0';

#ifdef VK_WIN32
  char *ctx;
  char *sr = strtok_s(s, " ;,", &ctx);
  char *sg = strtok_s(0, " ;,", &ctx);
  char *sb = strtok_s(0, " ;,", &ctx);
  char *sa = strtok_s(0, " ;,", &ctx);
#else
  char *sr = strtok(s, " ;,");
  char *sg = strtok(0, " ;,");
  char *sb = strtok(0, " ;,");
  char *sa = strtok(0, " ;,");
#endif

  float r = (float)atof(sr);
  float g = (float)atof(sg);
  float b = (float)atof(sb);
  float a = (float)atof(sa);
  delete[] s;

  return vkColor4f(r, g, b, a);
}

vkAssetXMLLoader::vkAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

vkAssetXMLLoader::~vkAssetXMLLoader()
{

}

bool vkAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("asset");
}

IObject *vkAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  TiXmlElement *dataElement = FindElementByTagName(element, locator.GetResourceName());
  if (!dataElement)
  {
    return 0;
  }

  TiXmlElement *firstChild = dataElement->FirstChildElement();
  if (!firstChild)
  {
    return 0;
  }

  return vkResourceManager::Get()->Load(firstChild, locator);
}