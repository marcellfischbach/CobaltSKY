
#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/ifile.hh>
#include <valkyrie/core/vkassetinputstream.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/core/resource/iassetloader.hh>
#include <valkyrie/core/resource/ifileloader.hh>
#include <valkyrie/core/resource/ixmlloader.hh>
#include <valkyrie/core/resource/vkassetfileloader.hh>
#include <valkyrie/core/resource/vkassetxmlloader.hh>
#include <valkyrie/core/resource/vkbasexmlloader.hh>
#include <valkyrie/core/resource/vkxmlfileloader.hh>

#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkvector.hh>
#include <tixml\tinyxml.h>
#include <map>
#include <vector>
#include <valkyrie/core/vkresourcemanager.refl.hh>





class VKE_API vkResourceManager
{

public:
  static vkResourceManager *Get();
  /**
   * \brief Register a user defined resource manager.
   *
   * When needing a differen resource manager (adding or overriding functions)
   * you can register your own implementation of the resource manager here.
   */
  static void Register(vkResourceManager *resourceManager);

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
  virtual iObject *Load(const vkResourceLocator &locator, iObject *userData = 0);
  virtual const vkClass *EvalClass(const vkResourceLocator &locator, iObject *userData = 0) const;

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
  virtual iObject *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0);

  virtual const vkClass *EvalClass(iFile *file, const vkResourceLocator &locator, iObject *userData = 0) const;

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
  virtual iObject *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0);


  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0) const;

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
  virtual iObject *Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0);
  virtual const vkClass *EvalClass(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0) const;

  template<typename T>
  T *Load(const vkResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(locator, userData);
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
  T *Load(iFile *file, const vkResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(file, locator, userData);
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
  T *Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(element, locator, userData);
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
  T *Load(const vkString &typeID, vkAssetInputStream &inputStream, const vkResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(typeID, inputStream, locator, userData);
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
  virtual iObject *Get(const vkResourceLocator &resourceLocator) const;


  /**
   * \brief Get the resource locator for the given object
   *
   * \param object The object for which to query the resource locator
   *
   * \return The resource locator
   */
  virtual vkResourceLocator GetLocator(iObject *object) const;

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
  virtual iObject *GetOrLoad(const vkResourceLocator &resourceLocator, iObject *userData = 0);

  virtual iObject *Aquire(const vkResourceLocator &resourceLocator, iObject *userData = 0, vkResourceLoadingMode mode = eRLM_Shared);



  template<typename T>
  T *Get(const vkResourceLocator &resourceLocator) const
  {
    iObject *object = Get(resourceLocator);
    if (object)
    {
      T *t_instance = vkQueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *GetOrLoad(const vkResourceLocator &resourceLocator, iObject *userData = 0)
  {
    iObject *object = GetOrLoad(resourceLocator, userData);
    if (object)
    {
      T *t_instance = vkQueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Aquire(const vkResourceLocator &resourceLocator, iObject *userData = 0, vkResourceLoadingMode mode = eRLM_Shared)
  {
    iObject *object = Aquire(resourceLocator, userData, mode);
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

  virtual void RegisterLoader(iXMLLoader *loader);
  virtual void RegisterLoader(iFileLoader *loader);
  virtual void RegisterLoader(iAssetLoader *loader);

  virtual bool RegisterObject(const vkResourceLocator &locator, iObject *object);
  virtual void UnregisterObject(const vkResourceLocator &locator);
  virtual void UnregisterObject(iObject *object);

  virtual void RegisterResource(iObject *object, const vkResourceLocator &locator);
  virtual void UnregisterResource(iObject *object);


protected:
  vkResourceManager();
  virtual ~vkResourceManager() {}

  virtual iFile *Open(const vkResourceLocator &locator) const;

protected:

  std::vector<iFileLoader*> m_fileLoaders;
  std::vector<iXMLLoader*> m_xmlLoaders;
  std::vector<iAssetLoader*> m_assetLoaders;

  std::map<vkResourceLocator, iObject*> m_objects;
  std::map<iObject*, vkResourceLocator> m_resources;

  static vkResourceManager *s_instance;
};


