#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <csrefl/class.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/cstypes.hh>
#include <vector>
#include <float.h>

#include <cobalt/entity/csentity.refl.hh>



struct iPhysicsBody;


namespace cs
{
struct iGraphics;
struct iEntityScan;
struct ScanConfig;

class BoundingBox;
class Clipper;
class EntityScene;
class EntityState;
class SpatialState;
class Transformation;


/**
 * \ingroup entity
 */
CS_CLASS()
class CSE_API Entity : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  Entity();
  virtual ~Entity();

  csID GetID() const;

  cs::EntityScene* GetScene();
  const cs::EntityScene* GetScene() const;

  CS_FUNCTION()
    void SetName(const std::string & name);
  CS_FUNCTION()
    const std::string& GetName() const;


  cs::EntityState* GetState(csID id);
  const cs::EntityState* GetState(csID id) const;
  cs::EntityState* GetState(const std::string & name);
  const cs::EntityState* GetState(const std::string & name) const;

  std::vector<cs::EntityState*> FindStates(const cs::Class * cls) const;
  cs::EntityState* FindState(const cs::Class * cls) const;

  template<typename T>
  T* FindState() const
  {
    cs::EntityState* state = FindState(T::GetStaticClass());
    if (!state)
    {
      return 0;
    }
    return cs::QueryClass<T>(state);
  }



  void SetRootState(cs::SpatialState * rootState);
  void AddState(cs::EntityState * state);
  void AddState(cs::SpatialState * state, cs::SpatialState * parentState);
  cs::SpatialState* GetRootState();
  const cs::SpatialState* GetRootState() const;

  void RemoveState(cs::EntityState * state);

  cs::Entity* GetParentEntity();
  const cs::Entity* GetParentEntity() const;

  const cs::BoundingBox& GetBoundingBox() const;
  cs::Transformation GetTransformation();
  void FinishTransformation();
  void UpdateBoundingBox();

  static csID GetNextID();

  virtual void Scan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);

  void AttachEntity(cs::Entity * entity, cs::SpatialState * parentState = 0);
  void AttachEntity(cs::Entity * entity, const std::string & parentStateName);
  void DetachEntity(cs::Entity * entity);


  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

  virtual void Update(float tpf);

  /**
  * \name Live-cycles
  * @{
  */
  void Create();
  void Assemble();
  void AttachToEntity(cs::Entity * entity);
  void AttachToScene(cs::EntityScene * scene);
  void DetachFromScene(cs::EntityScene * scene);
  void DetachFromEntity(cs::Entity * entity);
  void Disassemble();
  void Destroy();
  /**
  * @}
  */
protected:
  virtual void UpdateScene(cs::EntityScene * scene) { }
  void PerformTransformation();

  /**
  * \name Live-cycle event listeners
  * @{
  */

  virtual void OnCreated();
  virtual void OnAssembled();
  virtual void OnAttachedToEntity(cs::Entity * entity);
  virtual void OnAttachedToScene(cs::EntityScene * scene);
  virtual void OnDetachedFromScene(cs::EntityScene * scene);
  virtual void OnDetachedFromEntity(cs::Entity * entity);
  virtual void OnDisassembled();
  virtual void OnDestroyed();
  /**
  * @}
  */

private:
  csID m_id;

  CS_PROPERTY(name = Name)
    std::string m_name;

  cs::SpatialState * m_rootState;
  std::vector<cs::EntityState*> m_states;

  bool m_created;
  bool m_assemabled;
  cs::EntityScene * m_scene;
  cs::Entity * m_parentEntity;
  std::vector<cs::Entity*> m_children;
};


CS_CLASS()
class CSE_API EntityWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::Entity, EntityWrapper, cs::ResourceWrapper);
};

}

CS_FORCEINLINE csID cs::Entity::GetID() const
{
  return m_id;
}

CS_FORCEINLINE cs::EntityScene *cs::Entity::GetScene()
{
  return m_scene;
}

CS_FORCEINLINE const cs::EntityScene *cs::Entity::GetScene() const
{
  return m_scene;
}


CS_FORCEINLINE void cs::Entity::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE const std::string &cs::Entity::GetName() const
{
  return m_name;
}


CS_FORCEINLINE cs::SpatialState *cs::Entity::GetRootState()
{
  return m_rootState;
}

CS_FORCEINLINE const cs::SpatialState *cs::Entity::GetRootState() const
{
  return m_rootState;
}


CS_FORCEINLINE cs::Entity *cs::Entity::GetParentEntity()
{
  return m_parentEntity;
}

CS_FORCEINLINE const cs::Entity *cs::Entity::GetParentEntity() const
{
  return m_parentEntity;
}


