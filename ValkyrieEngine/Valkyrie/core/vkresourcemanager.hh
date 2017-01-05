
#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/ifile.hh>
#include <Valkyrie/core/vkassetinputstream.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcelocator.hh>
#include <Valkyrie/core/vkstring.hh>
#include <Valkyrie/core/resource/iassetloader.hh>
#include <Valkyrie/core/resource/ifileloader.hh>
#include <Valkyrie/core/resource/ixmlloader.hh>
#include <Valkyrie/core/resource/vkassetfileloader.hh>
#include <Valkyrie/core/resource/vkassetxmlloader.hh>
#include <Valkyrie/core/resource/vkbasexmlloader.hh>
#include <Valkyrie/core/resource/vkxmlfileloader.hh>

#include <Valkyrie/Math/vkcolor4f.hh>
#include <Valkyrie/Math/vkvector.hh>
#include <tixml\tinyxml.h>
#include <map>
#include <vector>
#include <Valkyrie/core/vkresourcemanager.refl.hh>





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
  const vkClass *EvalClass(const vkResourceLocator &locator, IObject *userData = 0) const;

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

  const vkClass *EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData = 0) const;

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


  const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  /**
  * \brief Load an object from the \a asset \a file.
  *
  * IMPORTANT: The caller is the owner of the returned object.
  *
  * \param name The typeIDwithin the asset file
  * \param inputStream The data stream from where the data sould be loaded
  * \param locator The origin location from where the resource should be loaded
  * \param userData An optional user data that the loader can use.
  *
  * \return The object
  */
  IObject *Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;
  const vkClass *EvalClass(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const;

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

  template<typename T>
  T *Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData = 0) const
  {
    IObject *object = Load(typeID, inputStream, locator, userData);
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
   * \brief Get the resource locator for the given object
   *
   * \param object The object for which to query the resource locator
   *
   * \return The resource locator
   */
  vkResourceLocator Get(IObject *object) const;

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
  void RegisterLoader(IAssetLoader *loader);

  bool RegisterObject(const vkResourceLocator &locator, IObject *object);
  void DeregisterObject(const vkResourceLocator &locator);
  void DeregisterObject(IObject *object);

private:
  vkResourceManager();

  std::vector<IFileLoader*> m_fileLoaders;
  std::vector<IXMLLoader*> m_xmlLoaders;
  std::vector<IAssetLoader*> m_assetLoaders;

  std::map<vkResourceLocator, IObject*> m_objects;
  std::map<IObject*, vkResourceLocator> m_resources;

};


