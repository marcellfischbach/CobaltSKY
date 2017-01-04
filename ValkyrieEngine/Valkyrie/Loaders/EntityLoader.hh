#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkresourcemanager.hh>
#include <Valkyrie/Loaders/EntityLoader.refl.hh>

class vkEntity;
class vkEntityState;

VK_CLASS()
class vkEntityStateLoaderData : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
  vkEntityStateLoaderData () : vkObject () { }
  virtual ~vkEntityStateLoaderData() { }

public:
  vkEntityState *state;
  vkEntity *entity;
};



VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkEntityMasterXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkEntityMasterXMLLoader();
  virtual ~vkEntityMasterXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkEntityStateMasterXMLLoader : public VK_SUPER(vkBaseXMLLoader)
{
  VK_CLASS_GEN;
public:
  vkEntityStateMasterXMLLoader();
  virtual ~vkEntityStateMasterXMLLoader();


  virtual bool CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual const vkClass *EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;
  virtual IObject *Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData = 0) const;



};


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkEntityXMLLoader: public VK_SUPER(vkBaseXMLLoader)
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


VK_CLASS()
/**
* \ingroup loading
*/
class VKE_API vkEntityStateXMLLoader : public VK_SUPER(vkBaseXMLLoader)
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
class VKE_API vkSpatialStateXMLLoader : public VK_SUPER(vkEntityStateXMLLoader)
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
class VKE_API vkRenderStateXMLLoader : public VK_SUPER(vkSpatialStateXMLLoader)
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
class VKE_API vkStaticMeshStateXMLLoader : public VK_SUPER(vkRenderStateXMLLoader)
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
class VKE_API vkColliderStateXMLLoader : public VK_SUPER(vkSpatialStateXMLLoader)
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
class VKE_API vkBaseColliderStateXMLLoader : public VK_SUPER(vkColliderStateXMLLoader)
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
class VKE_API vkDynamicColliderStateXMLLoader : public VK_SUPER(vkBaseColliderStateXMLLoader)
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
class VKE_API vkStaticColliderStateXMLLoader : public VK_SUPER(vkBaseColliderStateXMLLoader)
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
class VKE_API vkJointStateXMLLoader : public VK_SUPER(vkSpatialStateXMLLoader)
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
class VKE_API vkHingeJointStateXMLLoader : public VK_SUPER(vkJointStateXMLLoader)
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

