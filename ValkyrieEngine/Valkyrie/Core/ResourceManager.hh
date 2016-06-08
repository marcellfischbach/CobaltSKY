
#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Math/Vector.hh>
#include <tixml\tinyxml.h>
#include <map>
#include <vector>
#include <Valkyrie/Core/ResourceManager.refl.hh>



class VKE_API vkResourceLocator
{
public:
  vkResourceLocator(const vkString &resourceFile = "", const vkString &resourceName = "");
  
  const vkString &GetResourceFile() const;
  const vkString &GetResourceName() const;

  bool operator< (const vkResourceLocator &o) const;
  bool operator== (const vkResourceLocator &o) const;

  bool IsValid () const;

private:
  vkString m_resourceFile;
  vkString m_resourceName;

};

VK_INTERFACE();
struct VKE_API IFileLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
};


VK_CLASS();
class VKE_API vkAssetFileLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkAssetFileLoader();
  virtual ~vkAssetFileLoader();

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};


VK_INTERFACE();
struct VKE_API IAssetLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData = 0) = 0;

  virtual IObject *Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) = 0;
};



VK_INTERFACE();
struct VKE_API IXMLLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const = 0;
};

VK_CLASS();
class VKE_API vkXMLFileLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkXMLFileLoader();
  virtual ~vkXMLFileLoader();
  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;
  IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

};


VK_INTERFACE();
class VKE_API vkBaseXMLLoader : public IXMLLoader
{
  VK_CLASS_GEN_OBJECT;
protected:
  vkBaseXMLLoader();
  virtual ~vkBaseXMLLoader();

  TiXmlElement *FindElement(TiXmlElement *root, const vkString &elementName, const vkString &name = "") const;

  vkResourceLoadingMode GetResourceLoadingMode(TiXmlElement *element, vkResourceLoadingMode defaultMode = eRLM_Shared, vkResourceLoadingMode alterInline = eRLM_Inline) const;

  bool LoadBool(const char *str) const;
  float LoadFloat(const char *str) const;
  vkVector2f LoadVector2f(const char *str) const;
  vkVector3f LoadVector3f(const char *str) const;
  vkVector4f LoadVector4f(const char *str) const;
  vkColor4f LoadColor4f(const char *str) const;
};


class VKE_API vkResourceManager
{

public:
  static vkResourceManager *Get();

  /**
   * \brief Load an object from the \a locator.
   * 
   * IMPORTANT: The caller is the owner of the returned object.
   *
   * \param locator The location from where the resource should be loaded
   * \param userData An optional user data that the loader can use.
   *
   * \return The object
   */
  IObject *Load(const vkResourceLocator &locator, IObject *userData = 0) const;

  /**
  * \brief Load an object from the \a file.
  *
  * IMPORTANT: The caller is the owner of the returned object.
  *
  * \param file The file where resource should be read from
  * \param locator The origin location from where the resource should be loaded
  * \param userData An optional user data that the loader can use.
  *
  * \return The object
  */
  IObject *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

  /**
  * \brief Load an object from the \a file.
  *
  * IMPORTANT: The caller is the owner of the returned object.
  *
  * \param element The XML-element where resource should be read from
  * \param locator The origin location from where the resource should be loaded
  * \param userData An optional user data that the loader can use.
  *
  * \return The object
  */
  IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  template<typename T>
  T *Load(const vkResourceLocator &locator, IObject *userData = 0) const
  {
    IObject *object = Load(locator, userData);
    if (object)
    {
      T* t_instance = vkQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const
  {
    IObject *object = Load(file, locator, userData);
    if (object)
    {
      T* t_instance = vkQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const
  {
    IObject *object = Load(element, locator, userData);
    if (object)
    {
      T* t_instance = vkQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  /**
   * \brief Get the object from the resource cache.
   * 
   * IMPORTANT: The caller is not owner of the returned object. When the caller stores the object
   *            he must take ownership via \AddRef on the returned object.
   *
   * \param resourceLocator The internal name of the resource
   * 
   * \return The object or \a null if there is no such object registered.
   */
  IObject *Get(const vkResourceLocator &resourceLocator) const;

  /**
  * \brief Get the object from the resource cache. 
  * If the object is not in the cache it is loaded and than put into cache.
  *
  * IMPORTANT: The caller is not owner of the returned object. When the caller stores the object
  *            he must take ownership via \AddRef on the returned object.
  *
  * \param resourceLocator The internal name of the resource
  *
  * \return The object or \a null if there is no such object registered.
  */
  IObject *GetOrLoad(const vkResourceLocator &resourceLocator, IObject *userData = 0);

  VK_FORCEINLINE IObject *Aquire(const vkResourceLocator &resourceLocator, IObject *userData = 0, vkResourceLoadingMode mode = eRLM_Shared)
  {
    switch (mode)
    {
    case eRLM_Shared:
      return GetOrLoad(resourceLocator, userData);
    case eRLM_Instance:
      return Load(resourceLocator, userData);
    }
    return 0;
  }




  template<typename T>
  T *Get(const vkResourceLocator &resourceLocator) const
  {
    IObject *object = Get(resourceLocator);
    if (object)
    {
      T *t_instance = vkQueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *GetOrLoad(const vkResourceLocator &resourceLocator, IObject *userData = 0)
  {
    IObject *object = GetOrLoad(resourceLocator, userData);
    if (object)
    {
      T *t_instance = vkQueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Aquire(const vkResourceLocator &resourceLocator, IObject *userData = 0, vkResourceLoadingMode mode = eRLM_Shared)
  {
    IObject *object = Aquire(resourceLocator, userData, mode);
    if (object)
    {
      T *t_instance = vkQueryClass<T>(object);
      if (!t_instance && mode == eRLM_Instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  void RegisterLoader(IXMLLoader *loader);
  void RegisterLoader(IFileLoader *loader);

  bool RegisterObject(const vkResourceLocator &locator, IObject *object);
  void DeregisterObject(const vkResourceLocator &locator);

private:
  vkResourceManager();

  std::vector<IXMLLoader*> m_xmlLoaders;
  std::vector<IFileLoader*> m_fileLoaders;

  std::map<vkResourceLocator, IObject*> m_objects;

};


