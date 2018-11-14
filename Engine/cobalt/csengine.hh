#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/physics/iphysicssystem.hh>
#include <cobalt/window/iwindow.hh>





class CSE_API csEngine
{
public:
  csEngine();

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

  inline csResourceWrapper *Get(const std::string &resourceName, bool forceInstance = false) const
  {
    return Get(csResourceLocator(resourceName), forceInstance);
  }
  template<typename T>
  T *Get(const std::string &resourceName, bool forceInstance = false) const
  {
    return Get<T>(csResourceLocator(resourceName), forceInstance);
  }


  inline csResourceWrapper *Get(const csResourceLocator &locator, bool forceInstance = false) const
  {
    return csResourceManager::Get()->Aquire(locator, forceInstance ? eRLM_Instance : eRLM_Shared);
  }

  template<typename T>
  T *Get(const csResourceLocator &locator, bool forceInstance = false) const
  {
    return csResourceManager::Get()->Aquire<T>(locator, forceInstance ? eRLM_Instance : eRLM_Shared);
  }

  virtual iIndexBuffer *CreateIndexBuffer(csSize size, const void *data, csBufferDataMode mode);
  virtual iVertexBuffer *CreateVertexBuffer(csSize size, const void *data, csBufferDataMode mode);
  virtual iVertexDeclaration *CreateVertexDeclaration(const csVertexElement *elements);
  virtual iRenderTarget *CreateRenderTarget();
  virtual iSampler *CreateSampler();
  virtual iTexture2D *CreateTexture2D(csPixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps);
  virtual iTexture2DArray *CreateTexture2DArray(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps);
  virtual iTextureCube *CreateTextureCube(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth);
  virtual iShader *CreateShader(const std::string &vertexCode, const std::string &tessCtrl, const std::string &tessEval, const std::string &geometry, const std::string &fragmentCode);


  virtual iPhysicsScene *CreateScene();
  virtual iPhysicsShape *CreateShape(const csPhysGeometry &geometry);
  virtual iPhysicsDynamicCollider *CreateDynamicCollider();
  virtual iPhysicsStaticCollider *CreateStaticCollider();
  virtual iPhysicsTriggerCollider *CreateTriggerCollider();
  virtual iPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();
  virtual iPhysicsJoint *CreateJoint(csPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB);
  /** 
   * @}
   */

  /// don't use directly
  static csEngine *static_instance;
private:

  iWindow *m_window;  
  iGraphics *m_renderer;
  iPhysicsSystem *m_physicsSystem;

};


CS_FORCEINLINE iWindow *csEngine::GetWindow() const
{
  return m_window;
}

CS_FORCEINLINE iGraphics *csEngine::GetRenderer() const
{
  return m_renderer;
}

CS_FORCEINLINE iPhysicsSystem *csEngine::GetPhysicsSystem() const
{
  return m_physicsSystem;
}


#define csEng (csEngine::static_instance)
