#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csdefs.hh>
#include <vector>
#include <float.h>

#include <cobalt/entity/csentity.refl.hh>


class csBoundingBox;
class csClipper;
class csEntityScene;
class csEntityState;
class csSpatialState;
class csTransformation;

struct csScanConfig;
struct iEntityScan;
struct iGraphics;
struct iPhysicsBody;


/**
 * \ingroup entity
 */
CS_CLASS()
class CSE_API csEntity : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csEntity();
  virtual ~csEntity();

  csID GetID() const;

  csEntityScene *GetScene();
  const csEntityScene *GetScene() const;

  CS_FUNCTION()
  void SetName(const std::string &name);
  CS_FUNCTION()
  const std::string &GetName() const;


  csEntityState *GetState(csID id);
  const csEntityState *GetState(csID id) const;
  csEntityState *GetState(const std::string &name);
  const csEntityState *GetState(const std::string &name) const;

  std::vector<csEntityState*> FindStates(const csClass *cls) const;
  csEntityState* FindState(const csClass *cls) const;

  template<typename T>
  T* FindState() const
  {
    csEntityState *state = FindState(T::GetStaticClass());
    if (!state)
    {
      return 0;
    }
    return csQueryClass<T>(state);
  }


  void SetRootState(csSpatialState *rootState);
  void AddState(csEntityState *state);
  void AddState(csSpatialState *state, csSpatialState *parentState);
  csSpatialState *GetRootState();
  const csSpatialState *GetRootState() const;

  void RemoveState(csEntityState *state);

  csEntity *GetParentEntity() ;
  const csEntity *GetParentEntity() const;

  const csBoundingBox &GetBoundingBox() const;
  csTransformation GetTransformation();
  void FinishTransformation();
  void UpdateBoundingBox();

  static csID GetNextID();

  virtual void Scan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);

  void AttachEntity(csEntity *entity, csSpatialState *parentState = 0);
  void AttachEntity(csEntity *entity, const std::string &parentStateName);
  void DetachEntity(csEntity *entity);


  void SetClippingRange(float min = -FLT_MAX, float max = FLT_MAX);

  virtual void Update(float tpf);

  /**
  * \name Live-cycles
  * @{
  */
  void Create();
  void Assemble();
  void AttachToEntity(csEntity *entity);
  void AttachToScene(csEntityScene *scene);
  void DetachFromScene(csEntityScene *scene);
  void DetachFromEntity(csEntity *entity);
  void Disassemble();
  void Destroy();
  /**
  * @}
  */
protected:
  virtual void UpdateScene(csEntityScene *scene) { }
  void PerformTransformation();

  /**
  * \name Live-cycle event listeners
  * @{
  */

  virtual void OnCreated();
  virtual void OnAssembled();
  virtual void OnAttachedToEntity(csEntity *entity);
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);
  virtual void OnDetachedFromEntity(csEntity *entity);
  virtual void OnDisassembled();
  virtual void OnDestroyed();
  /**
  * @}
  */

private:
  csID m_id;

  CS_PROPERTY(name=Name)
  std::string m_name;

  csSpatialState *m_rootState;
  std::vector<csEntityState*> m_states;

  bool m_created;
  bool m_assemabled;
  csEntityScene *m_scene;
  csEntity *m_parentEntity;
  std::vector<csEntity*> m_children;
};

CS_CLASS()
class CSE_API csEntityWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csEntity, csEntityWrapper, csResourceWrapper);
};

CS_FORCEINLINE csID csEntity::GetID() const
{
  return m_id;
}

CS_FORCEINLINE csEntityScene *csEntity::GetScene()
{
  return m_scene;
}

CS_FORCEINLINE const csEntityScene *csEntity::GetScene() const
{
  return m_scene;
}


CS_FORCEINLINE void csEntity::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE const std::string &csEntity::GetName() const
{
  return m_name;
}


CS_FORCEINLINE csSpatialState *csEntity::GetRootState()
{
  return m_rootState;
}

CS_FORCEINLINE const csSpatialState *csEntity::GetRootState() const
{
  return m_rootState;
}


CS_FORCEINLINE csEntity *csEntity::GetParentEntity()
{
  return m_parentEntity;
}

CS_FORCEINLINE const csEntity *csEntity::GetParentEntity() const
{
  return m_parentEntity;
}


