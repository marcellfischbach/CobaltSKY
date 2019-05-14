
#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/ifile.hh>
#include <cobalt/core/csassetinputstream.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/core/resource/iassetloader.hh>
#include <cobalt/core/resource/icsfloader.hh>
#include <cobalt/core/resource/ifileloader.hh>
#include <cobalt/core/resource/csassetcsfloader.hh>
#include <cobalt/core/resource/csbasecsfloader.hh>
#include <cobalt/core/resource/cscsffileloader.hh>

#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <csfile/csffile.hh>
#include <map>
#include <vector>
#include <string>






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
  virtual csResourceWrapper *Load(const csResourceLocator &locator);
  virtual const cs::Class *EvalClass(const csResourceLocator &locator) const;

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
  virtual csResourceWrapper *Load(iFile *file, const csResourceLocator &locator);

  virtual const cs::Class *EvalClass(iFile *file, const csResourceLocator &locator) const;

 
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
  virtual csResourceWrapper *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr);
  virtual const cs::Class *EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

  virtual csResourceWrapper *Load(const csfBlob *blob, const csResourceLocator &locator, cs::iObject *userData = nullptr);
  virtual const cs::Class *EvalClass(const csfBlob *blob, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

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
  virtual csResourceWrapper *Load(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr);
  virtual const cs::Class *EvalClass(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr) const;

  template<typename T>
  T *Load(const csResourceLocator &locator)
  {
    csResourceWrapper *object = Load(locator);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(iFile *file, const csResourceLocator &locator)
  {
    csResourceWrapper *object = Load(file, locator);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }


  template<typename T>
  T *Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData = nullptr)
  {
    csResourceWrapper *object = Load(entry, locator, userData);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Load(const csfBlob *blob, const csResourceLocator &locator, cs::iObject *userData = nullptr)
  {
    csResourceWrapper *object = Load(blob, locator, userData);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      if (!t_instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }


  template<typename T>
  T *Load(const std::string &typeID, csAssetInputStream &inputStream, const csResourceLocator &locator, cs::iObject *userData = nullptr)
  {
    csResourceWrapper *object = Load(typeID, inputStream, locator, userData);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
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
  virtual csResourceWrapper *Get(const csResourceLocator &resourceLocator) const;

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
  virtual csResourceWrapper *GetOrLoad(const csResourceLocator &resourceLocator);

  virtual csResourceWrapper *Aquire(const csResourceLocator &resourceLocator, csResourceLoadingMode mode = eRLM_Shared);



  template<typename T>
  T *Get(const csResourceLocator &resourceLocator) const
  {
    csResourceWrapper *object = Get(resourceLocator);
    if (object)
    {
      T *t_instance = cs::QueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *GetOrLoad(const csResourceLocator &resourceLocator)
  {
    csResourceWrapper *object = GetOrLoad(resourceLocator);
    if (object)
    {
      T *t_instance = cs::QueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T *Aquire(const csResourceLocator &resourceLocator, csResourceLoadingMode mode = eRLM_Shared)
  {
    csResourceWrapper *object = Aquire(resourceLocator, mode);
    if (object)
    {
      T *t_instance = cs::QueryClass<T>(object);
      if (!t_instance && mode == eRLM_Instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  virtual void Reload(const csResourceLocator& locator);

  virtual void RegisterLoader(iCSFLoader *loader);
  virtual void RegisterLoader(iFileLoader *loader);
  virtual void RegisterLoader(iAssetLoader *loader);

  virtual bool RegisterObject(const csResourceLocator &locator, csResourceWrapper *object);
  virtual void UnregisterObject(const csResourceLocator &locator);
  virtual void UnregisterObject(csResourceWrapper *object);

  virtual void RenameResource(const csResourceLocator &from, const csResourceLocator &to);


protected:
  csResourceManager();
  virtual ~csResourceManager() {}

  virtual iFile *Open(const csResourceLocator &locator) const;

protected:

  std::vector<iFileLoader*> m_fileLoaders;
  std::vector<iCSFLoader*> m_csfLoaders;
  std::vector<iAssetLoader*> m_assetLoaders;

  std::map<csResourceLocator, csResourceWrapper*> m_objects;

  static csResourceManager *s_instance;
};


