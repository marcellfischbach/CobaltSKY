
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


namespace cs
{


class CSE_API ResourceManager
{

public:
  static cs::ResourceManager* Get();
  /**
   * \brief Register a user defined resource manager.
   *
   * When needing a differen resource manager (adding or overriding functions)
   * you can register your own implementation of the resource manager here.
   */
  static void Register(cs::ResourceManager* resourceManager);

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
  virtual cs::ResourceWrapper* Load(const cs::ResourceLocator& locator);
  virtual const cs::Class* EvalClass(const cs::ResourceLocator& locator) const;

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
  virtual cs::ResourceWrapper* Load(cs::iFile* file, const cs::ResourceLocator& locator);

  virtual const cs::Class* EvalClass(cs::iFile* file, const cs::ResourceLocator& locator) const;


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
  virtual cs::ResourceWrapper* Load(const csfEntry* entry, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr);
  virtual const cs::Class* EvalClass(const csfEntry* entry, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const;

  virtual cs::ResourceWrapper* Load(const csfBlob* blob, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr);
  virtual const cs::Class* EvalClass(const csfBlob* blob, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const;

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
  virtual cs::ResourceWrapper* Load(const std::string& typeID, cs::AssetInputStream& inputStream, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr);
  virtual const cs::Class* EvalClass(const std::string& typeID, cs::AssetInputStream& inputStream, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr) const;

  template<typename T>
  T* Load(const cs::ResourceLocator& locator)
  {
    cs::ResourceWrapper* object = Load(locator);
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
  T* Load(cs::iFile* file, const cs::ResourceLocator& locator)
  {
    cs::ResourceWrapper* object = Load(file, locator);
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
  T* Load(const csfEntry* entry, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr)
  {
    cs::ResourceWrapper* object = Load(entry, locator, userData);
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
  T* Load(const csfBlob* blob, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr)
  {
    cs::ResourceWrapper* object = Load(blob, locator, userData);
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
  T* Load(const std::string& typeID, cs::AssetInputStream& inputStream, const cs::ResourceLocator& locator, cs::iObject* userData = nullptr)
  {
    cs::ResourceWrapper* object = Load(typeID, inputStream, locator, userData);
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
  virtual cs::ResourceWrapper* Get(const cs::ResourceLocator& resourceLocator) const;

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
  virtual cs::ResourceWrapper* GetOrLoad(const cs::ResourceLocator& resourceLocator);

  virtual cs::ResourceWrapper* Aquire(const cs::ResourceLocator& resourceLocator, cs::eResourceLoadingMode mode = cs::eRLM_Shared);



  template<typename T>
  T* Get(const cs::ResourceLocator& resourceLocator) const
  {
    cs::ResourceWrapper* object = Get(resourceLocator);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T* GetOrLoad(const cs::ResourceLocator& resourceLocator)
  {
    cs::ResourceWrapper* object = GetOrLoad(resourceLocator);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      return t_instance;
    }
    return 0;
  }

  template<typename T>
  T* Aquire(const cs::ResourceLocator& resourceLocator, cs::eResourceLoadingMode mode = cs::eRLM_Shared)
  {
    cs::ResourceWrapper* object = Aquire(resourceLocator, mode);
    if (object)
    {
      T* t_instance = cs::QueryClass<T>(object);
      if (!t_instance && mode == cs::eRLM_Instance)
      {
        object->Release();
      }
      return t_instance;
    }
    return 0;
  }

  virtual void Reload(const cs::ResourceLocator& locator);

  virtual void RegisterLoader(cs::iCSFLoader* loader);
  virtual void RegisterLoader(cs::iFileLoader* loader);
  virtual void RegisterLoader(cs::iAssetLoader* loader);

  virtual bool RegisterObject(const cs::ResourceLocator& locator, cs::ResourceWrapper* object);
  virtual void UnregisterObject(const cs::ResourceLocator& locator);
  virtual void UnregisterObject(cs::ResourceWrapper* object);

  virtual void RenameResource(const cs::ResourceLocator& from, const cs::ResourceLocator& to);


protected:
  ResourceManager();
  virtual ~ResourceManager() {}

  virtual cs::iFile* Open(const cs::ResourceLocator& locator) const;

protected:

  std::vector<cs::iFileLoader*> m_fileLoaders;
  std::vector<cs::iCSFLoader*> m_csfLoaders;
  std::vector<cs::iAssetLoader*> m_assetLoaders;

  std::map<cs::ResourceLocator, cs::ResourceWrapper*> m_objects;

  static cs::ResourceManager* s_instance;
};


}

