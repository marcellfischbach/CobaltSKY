
#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csassetinputstream.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/resource/iassetloader.hh>
#include <cobalt/core/resource/icsfloader.hh>
#include <cobalt/core/resource/ifileloader.hh>
#include <cobalt/core/resource/ixmlloader.hh>
#include <cobalt/core/resource/csassetxmlloader.hh>
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/resource/csbasexmlloader.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/core/resource/csxmlfileloader.hh>
#include <cobalt/core/resource/cscsffileloader.hh>

#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <tixml/tinyxml.h>
#include <csfile/csffile.hh>
#include <map>
#include <vector>
#include <string>

#include <cobalt/core/csresourcemanager.refl.hh>





class CSE_API csResourceManager
{

public:
  static csResourceManager *Get();
  /**
   * \brief Register a user defined resource manager.
   *
   * When needing a differen resource manager (adding or overriding functions)
   * you can register your own implementation of the resource manager here.
   */
  static void Register(csResourceManager *resourceManager);

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
  virtual iObject *Load(const csResourceLocator &locator, iObject *userData = 0);
  virtual const csClass *EvalClass(const csResourceLocator &locator, iObject *userData = 0) const;

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
  virtual iObject *Load(iFile *file, const csResourceLocator &locator, iObject *userData = 0);

  virtual const csClass *EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData = 0) const;

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
  virtual iObject *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0);


  virtual const csClass *EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0) const;

  /**
  * \brief Load an object from the \a csffile.
  *
  * IMPORTANT: The caller is the owner of the returned object.
  *
  * \param element The XML-element where resource should be read from
  * \param locator The origin location from where the resource should be loaded
  * \param userData An optional user data that the loader can use.
  *
  * \return The object
  */
  virtual iObject *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0);


  virtual const csClass *EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0) const;

  virtual iObject *Load(const csfBlob *blob, const csResourceLocator &locator, iObject *userData = 0);
  virtual const csClass *EvalClass(const csfBlob *blob, const csResourceLocator &locator, iObject *userData = 0) const;

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
  virtual iObject *Load(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0);
  virtual const csClass *EvalClass(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0) const;

  template<typename T>
  T *Load(const csResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(locator, userData);
    if (object)
    {
      T* t_instance = csQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(iFile *file, const csResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(file, locator, userData);
    if (object)
    {
      T* t_instance = csQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(element, locator, userData);
    if (object)
    {
      T* t_instance = csQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(entry, locator, userData);
    if (object)
    {
      T* t_instance = csQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(const csfBlob *blob, const csResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(blob, locator, userData);
    if (object)
    {
      T* t_instance = csQueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }


  template<typename T>
  T *Load(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData = 0)
  {
    iObject *object = Load(typeID, inputStream, locator, userData);
    if (object)
    {
      T* t_instance = csQueryClass<T>(object);
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
  virtual iObject *Get(const csResourceLocator &resourceLocator) const;


  /**
   * \brief Get the resource locator for the given object
   *
   * \param object The object for which to query the resource locator
   *
   * \return The resource locator
   */
  virtual csResourceLocator GetLocator(iObject *object) const;

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
  virtual iObject *GetOrLoad(const csResourceLocator &resourceLocator, iObject *userData = 0);

  virtual iObject *Aquire(const csResourceLocator &resourceLocator, iObject *userData = 0, csResourceLoadingMode mode = eRLM_Shared);



  template<typename T>
  T *Get(const csResourceLocator &resourceLocator) const
  {
    iObject *object = Get(resourceLocator);
    if (object)
    {
      T *t_instance = csQueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *GetOrLoad(const csResourceLocator &resourceLocator, iObject *userData = 0)
  {
    iObject *object = GetOrLoad(resourceLocator, userData);
    if (object)
    {
      T *t_instance = csQueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Aquire(const csResourceLocator &resourceLocator, iObject *userData = 0, csResourceLoadingMode mode = eRLM_Shared)
  {
    iObject *object = Aquire(resourceLocator, userData, mode);
    if (object)
    {
      T *t_instance = csQueryClass<T>(object);
      if (!t_instance && mode == eRLM_Instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  virtual void RegisterLoader(iXMLLoader *loader);
  virtual void RegisterLoader(iCSFLoader *loader);
  virtual void RegisterLoader(iFileLoader *loader);
  virtual void RegisterLoader(iAssetLoader *loader);

  virtual bool RegisterObject(const csResourceLocator &locator, iObject *object);
  virtual void UnregisterObject(const csResourceLocator &locator);
  virtual void UnregisterObject(iObject *object);

  virtual void RegisterResource(iObject *object, const csResourceLocator &locator);
  virtual void UnregisterResource(iObject *object);

  virtual void RenameResource(const csResourceLocator &from, const csResourceLocator &to);


protected:
  csResourceManager();
  virtual ~csResourceManager() {}

  virtual iFile *Open(const csResourceLocator &locator) const;

protected:

  std::vector<iFileLoader*> m_fileLoaders;
  std::vector<iCSFLoader*> m_csfLoaders;
  std::vector<iXMLLoader*> m_xmlLoaders;
  std::vector<iAssetLoader*> m_assetLoaders;

  std::map<csResourceLocator, iObject*> m_objects;
  std::map<iObject*, csResourceLocator> m_resources;

  static csResourceManager *s_instance;
};


