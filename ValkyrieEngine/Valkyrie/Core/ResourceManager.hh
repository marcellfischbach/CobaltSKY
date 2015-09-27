
#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <tixml\tinyxml.h>
#include <vector>
#include <Valkyrie/Core/ResourceManager.refl.hh>

class VKE_API vkResourceLocator
{
public:
  vkResourceLocator(const vkString &resourceFile = "", const vkString &resourceName = "");
  
  const vkString &GetResourceFile() const;
  const vkString &GetResourceName() const;

  bool operator< (const vkResourceLocator &o) const;

private:
  vkString m_resourceFile;
  vkString m_resourceName;

};

VK_INTERFACE();
struct VKE_API IFileLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator) const = 0;
  virtual IObject *Load(IFile *file, const vkResourceLocator &locator) const = 0;
};


VK_INTERFACE();
struct VKE_API IXMLLoader : public IObject
{
  VK_CLASS_GEN;

  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator) const = 0;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator) const = 0;
};

VK_CLASS();
class VKE_API vkXMLFileLoader : public IFileLoader
{
  VK_CLASS_GEN_OBJECT;
public:
  vkXMLFileLoader();
  virtual ~vkXMLFileLoader();
  virtual bool CanLoad(IFile *file, const vkResourceLocator &locator) const;
  IObject *Load(IFile *file, const vkResourceLocator &locator) const;

};


class VKE_API vkResourceManager
{

public:
  static vkResourceManager *Get();

  IObject *Load(const vkResourceLocator &locator) const;
  IObject *Load(IFile *file, const vkResourceLocator &locator) const;
  IObject *Load(TiXmlElement *element, const vkResourceLocator &locator) const;

  template<typename T>
  T *Load(IFile *file, const vkResourceLocator &locator) const
  {
    IObject *object = Load(file, locator);
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
  T *Load(TiXmlElement *element, const vkResourceLocator &locator) const
  {
    IObject *object = Load(element, locator);
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

  void RegisterLoader(IXMLLoader *loader);
  void RegisterLoader(IFileLoader *loader);

private:
  vkResourceManager();

  std::vector<IXMLLoader*> m_xmlLoaders;
  std::vector<IFileLoader*> m_fileLoaders;


};