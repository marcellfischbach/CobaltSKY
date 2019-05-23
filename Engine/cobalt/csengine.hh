#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/physics/iphysicssystem.hh>
#include <cobalt/window/iwindow.hh>

namespace cs
{

class CSE_API Engine
{
public:
  Engine();

  void SetWindow(cs::iWindow* window);
  void SetRenderer(cs::iGraphics* renderer);
  void SetPhysicsSystem(cs::iPhysicsSystem* physicsSystem);

  cs::iWindow* GetWindow() const;
  cs::iGraphics* GetRenderer() const;
  cs::iPhysicsSystem* GetPhysicsSystem() const;

  bool Initialize();
  bool SetupTestScene();

  /**
   * \name Creators
   * @{
   */

  inline cs::ResourceWrapper* Get(const std::string& resourceName, bool forceInstance = false) const
  {
    return Get(cs::ResourceLocator(resourceName), forceInstance);
  }
  template<typename T>
  T* Get(const std::string& resourceName, bool forceInstance = false) const
  {
    return Get<T>(cs::ResourceLocator(resourceName), forceInstance);
  }


  inline cs::ResourceWrapper* Get(const cs::ResourceLocator& locator, bool forceInstance = false) const
  {
    return cs::ResourceManager::Get()->Aquire(locator, forceInstance ? cs::eRLM_Instance : cs::eRLM_Shared);
  }

  template<typename T>
  T* Get(const cs::ResourceLocator & locator, bool forceInstance = false) const
  {
    return cs::ResourceManager::Get()->Aquire<T>(locator, forceInstance ? cs::eRLM_Instance : cs::eRLM_Shared);
  }

  virtual cs::iIndexBuffer* CreateIndexBuffer(csSize size, const void* data, cs::eBufferDataMode mode);
  virtual cs::iVertexBuffer* CreateVertexBuffer(csSize size, const void* data, cs::eBufferDataMode mode);
  virtual cs::iVertexDeclaration* CreateVertexDeclaration(const cs::VertexElement * elements);
  virtual cs::iRenderTarget* CreateRenderTarget();
  virtual cs::iSampler* CreateSampler();
  virtual cs::iTexture2D* CreateTexture2D(cs::ePixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps);
  virtual cs::iTexture2DArray* CreateTexture2DArray(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps);
  virtual cs::iTextureCube* CreateTextureCube(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth);
  virtual cs::iShader* CreateShader(const std::string & vertexCode, const std::string & tessCtrl, const std::string & tessEval, const std::string & geometry, const std::string & fragmentCode);


  virtual cs::iPhysicsScene* CreateScene();
  virtual cs::iPhysicsShape* CreateShape(const cs::PhysGeometry & geometry);
  virtual cs::iPhysicsDynamicCollider* CreateDynamicCollider();
  virtual cs::iPhysicsStaticCollider* CreateStaticCollider();
  virtual cs::iPhysicsTriggerCollider* CreateTriggerCollider();
  virtual cs::iPhysicsCapsuleCharacterController* CreateCapsulseCharacterController();
  virtual cs::iPhysicsJoint* CreateJoint(cs::ePhysicsJointType type, cs::iPhysicsDynamicCollider * colliderA, cs::iPhysicsDynamicCollider * colliderB);
  /**
   * @}
   */

   /// don't use directly
  static cs::Engine* static_instance;
private:

  cs::iWindow* m_window;
  cs::iGraphics* m_renderer;
  cs::iPhysicsSystem* m_physicsSystem;

};

}

CS_FORCEINLINE cs::iWindow *cs::Engine::GetWindow() const
{
  return m_window;
}

CS_FORCEINLINE cs::iGraphics *cs::Engine::GetRenderer() const
{
  return m_renderer;
}

CS_FORCEINLINE cs::iPhysicsSystem *cs::Engine::GetPhysicsSystem() const
{
  return m_physicsSystem;
}


#define csEng (cs::Engine::static_instance)
