#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/EntityLoader.refl.hh>

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
class VKE_API vkEntityMasterXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityMasterXMLLoader();
  virtual ~vkEntityMasterXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};


VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityStateMasterXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityStateMasterXMLLoader();
  virtual ~vkEntityStateMasterXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};


VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityXMLLoader: public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityXMLLoader();
  virtual ~vkEntityXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  virtual const vkClass *GetLoadingClass() const;
};


VK_CLASS();
/**
* \ingroup loading
*/
class VKE_API vkEntityStateXMLLoader : public vkBaseXMLLoader
{
  VK_CLASS_GEN;
public:
  vkEntityStateXMLLoader();
  virtual ~vkEntityStateXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;

  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkSpatialStateXMLLoader : public vkEntityStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkSpatialStateXMLLoader();
  virtual ~vkSpatialStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkRenderStateXMLLoader : public vkSpatialStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkRenderStateXMLLoader();
  virtual ~vkRenderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkStaticMeshStateXMLLoader : public vkRenderStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkStaticMeshStateXMLLoader();
  virtual ~vkStaticMeshStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};



VK_CLASS()
class VKE_API vkColliderStateXMLLoader : public vkSpatialStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkColliderStateXMLLoader();
  virtual ~vkColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
class VKE_API vkBaseColliderStateXMLLoader : public vkColliderStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkBaseColliderStateXMLLoader();
  virtual ~vkBaseColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
class VKE_API vkDynamicColliderStateXMLLoader : public vkBaseColliderStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkDynamicColliderStateXMLLoader();
  virtual ~vkDynamicColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


VK_CLASS()
class VKE_API vkStaticColliderStateXMLLoader : public vkBaseColliderStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkStaticColliderStateXMLLoader();
  virtual ~vkStaticColliderStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};

VK_CLASS()
class VKE_API vkJointStateXMLLoader : public vkSpatialStateXMLLoader
{
  VK_CLASS_GEN;
public:
  vkJointStateXMLLoader();
  virtual ~vkJointStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;
};

VK_CLASS()
class VKE_API vkHingeJointStateXMLLoader : public vkJointStateXMLLoader
{
  VK_CLASS_GEN;
  vkHingeJointStateXMLLoader();
  virtual ~vkHingeJointStateXMLLoader();

  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *GetLoadingClass() const;

};


/**
* \ingroup loading
*/
class VKE_API vkEntityLoaderRegistry
{
public:
  static vkEntityLoaderRegistry *Get();

  void RegisterLoader(const vkEntityXMLLoader *entityLoader);
  void RegisterLoader(const vkEntityStateXMLLoader *entityLoader);
  const vkEntityXMLLoader *GetEntityLoader(const vkClass *clazz) const;
  const vkEntityStateXMLLoader *GetEntityStateLoader(const vkClass *clazz) const;

private:
  vkEntityLoaderRegistry();

  std::map<const vkClass*, const vkEntityXMLLoader*> m_entityLoaders;
  std::map<const vkClass*, const vkEntityStateXMLLoader*> m_entityStateLoaders;
};

