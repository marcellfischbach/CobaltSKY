#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>

class vkEntity;
class vkEntityState;

VK_CLASS()
class vkEntityStateLoaderData : public vkObject
{
  VK_CLASS_GEN;
  vkEntityStateLoaderData () : vkObject () { }
  virtual ~vkEntityStateLoaderData() { }

public:
  vkEntityState *state;
  vkEntity *entity;
};



VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityMasterLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityMasterLoader();
  virtual ~vkEntityMasterLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};


VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityStateMasterLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityStateMasterLoader();
  virtual ~vkEntityStateMasterLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};


VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityLoader: public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityLoader();
  virtual ~vkEntityLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  virtual const vkClass *GetLoadingClass() const;
};


VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityStateLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityStateLoader();
  virtual ~vkEntityStateLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkSpatialStateLoader : public vkEntityStateLoader
{
  VK_CLASS_GEN;
public:
  vkSpatialStateLoader();
  virtual ~vkSpatialStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkRenderStateLoader : public vkSpatialStateLoader
{
  VK_CLASS_GEN;
public:
  vkRenderStateLoader();
  virtual ~vkRenderStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkStaticMeshStateLoader : public vkRenderStateLoader
{
  VK_CLASS_GEN;
public:
  vkStaticMeshStateLoader();
  virtual ~vkStaticMeshStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};



VK_CLASS()
class VKE_API vkColliderStateLoader : public vkSpatialStateLoader
{
  VK_CLASS_GEN;
public:
  vkColliderStateLoader();
  virtual ~vkColliderStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
class VKE_API vkBaseColliderStateLoader : public vkColliderStateLoader
{
  VK_CLASS_GEN;
public:
  vkBaseColliderStateLoader();
  virtual ~vkBaseColliderStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
class VKE_API vkDynamicColliderStateLoader : public vkBaseColliderStateLoader
{
  VK_CLASS_GEN;
public:
  vkDynamicColliderStateLoader();
  virtual ~vkDynamicColliderStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
class VKE_API vkStaticColliderStateLoader : public vkBaseColliderStateLoader
{
  VK_CLASS_GEN;
public:
  vkStaticColliderStateLoader();
  virtual ~vkStaticColliderStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};

VK_CLASS()
class VKE_API vkJointStateLoader : public vkSpatialStateLoader
{
  VK_CLASS_GEN;
public:
  vkJointStateLoader();
  virtual ~vkJointStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;
};

VK_CLASS()
class VKE_API vkHingeJointStateLoader : public vkJointStateLoader
{
  VK_CLASS_GEN;
  vkHingeJointStateLoader();
  virtual ~vkHingeJointStateLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


/**
* \ingroup loading
*/
class VKE_API vkEntityLoaderRegistry
{
public:
  static vkEntityLoaderRegistry *Get();

  void RegisterLoader(const vkEntityLoader *entityLoader);
  void RegisterLoader(const vkEntityStateLoader *entityLoader);
  const vkEntityLoader *GetEntityLoader(const vkClass *clazz) const;
  const vkEntityStateLoader *GetEntityStateLoader(const vkClass *clazz) const;

private:
  vkEntityLoaderRegistry();

  std::map<const vkClass*, const vkEntityLoader*> m_entityLoaders;
  std::map<const vkClass*, const vkEntityStateLoader*> m_entityStateLoaders;
};

