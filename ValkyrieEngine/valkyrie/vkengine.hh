#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkdefs.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/physics/iphysicssystem.hh>
#include <valkyrie/window/iwindow.hh>





class VKE_API vkEngine
{
public:
  vkEngine();

  void SetWindow(iWindow *window);
  void SetRenderer(iGraphics *renderer);
  void SetPhysicsSystem(iPhysicsSystem *physicsSystem);

  iWindow *GetWindow() const;
  iGraphics *GetRenderer() const;
  iPhysicsSystem *GetPhysicsSystem() const;

  bool Initialize();
  bool SetupTestScene();

  /**
   * \name Creators
   * @{
   */

  inline iObject *Get(const vkString &resourceName, bool forceInstance = false) const
  {
    return Get(vkResourceLocator(resourceName), forceInstance);
  }
  template<typename T>
  T *Get(const vkString &resourceName, bool forceInstance = false) const
  {
    return Get<T>(vkResourceLocator(resourceName), forceInstance);
  }


  inline iObject *Get(const vkResourceLocator &locator, bool forceInstance = false) const
  {
    return vkResourceManager::Get()->Aquire(locator, 0, forceInstance ? eRLM_Instance : eRLM_Shared);
  }

  template<typename T>
  T *Get(const vkResourceLocator &locator, bool forceInstance = false) const
  {
    return vkResourceManager::Get()->Aquire<T>(locator, 0, forceInstance ? eRLM_Instance : eRLM_Shared);
  }

  virtual iIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual iVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual iVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements);
  virtual iRenderTarget *CreateRenderTarget();
  virtual iSampler *CreateSampler();
  virtual iTexture2D *CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps);
  virtual iTexture2DArray *CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps);
  virtual iTextureCube *CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth);
  virtual iShader *CreateShader(const vkString &vertexCode, const vkString &tessCtrl, const vkString &tessEval, const vkString &geometry, const vkString &fragmentCode);


  virtual iPhysicsScene *CreateScene();
  virtual iPhysicsShape *CreateShape(const vkPhysGeometry &geometry);
  virtual iPhysicsDynamicCollider *CreateDynamicCollider();
  virtual iPhysicsStaticCollider *CreateStaticCollider();
  virtual iPhysicsTriggerCollider *CreateTriggerCollider();
  virtual iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();
  virtual iPhysicsJoint *CreateJoint(vkPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB);
  /** 
   * @}
   */

  /// don't use directly
  static vkEngine *static_instance;
private:

  iWindow *m_window;  
  iGraphics *m_renderer;
  iPhysicsSystem *m_physicsSystem;

};


VK_FORCEINLINE iWindow *vkEngine::GetWindow() const
{
  return m_window;
}

VK_FORCEINLINE iGraphics *vkEngine::GetRenderer() const
{
  return m_renderer;
}

VK_FORCEINLINE iPhysicsSystem *vkEngine::GetPhysicsSystem() const
{
  return m_physicsSystem;
}


#define vkEng (vkEngine::static_instance)
