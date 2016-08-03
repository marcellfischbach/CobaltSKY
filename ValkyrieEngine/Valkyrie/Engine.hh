#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <Valkyrie/Window/IWindow.hh>





class VKE_API vkEngine
{
public:
  vkEngine();

  void SetWindow(IWindow *window);
  void SetRenderer(IGraphics *renderer);
  void SetPhysicsSystem(IPhysicsSystem *physicsSystem);

  IWindow *GetWindow() const;
  IGraphics *GetRenderer() const;
  IPhysicsSystem *GetPhysicsSystem() const;

  bool Initialize();
  bool SetupTestScene();

  /**
   * \name Creators
   * @{
   */

  inline IObject *Get(const vkString &resourceName, bool forceInstance = false) const
  {
    return Get(vkResourceLocator(resourceName), forceInstance);
  }
  template<typename T>
  T *Get(const vkString &resourceName, bool forceInstance = false) const
  {
    return Get<T>(vkResourceLocator(resourceName), forceInstance);
  }

  
  IObject *Get(const vkResourceLocator &locator, bool forceInstance = false) const;
  template<typename T>
  T *Get(const vkResourceLocator &locator, bool forceInstance = false) const
  {
    IObject *object = Get(locator, forceInstance);
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

  virtual IIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual IVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual IVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements);
  virtual IRenderTarget *CreateRenderTarget();
  virtual ISampler *CreateSampler();
  virtual ITexture2D *CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps);
  virtual ITexture2DArray *CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps);
  virtual ITextureCube *CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth);
  virtual IShader *CreateShader(const vkString &vertexCode, const vkString &tessCtrl, const vkString &tessEval, const vkString &geometry, const vkString &fragmentCode);


  virtual IPhysicsScene *CreateScene();
  virtual IPhysicsShape *CreateShape(const vkPhysGeometry &geometry);
  virtual IPhysicsDynamicCollider *CreateDynamicCollider();
  virtual IPhysicsStaticCollider *CreateStaticCollider();
  virtual IPhysicsTriggerCollider *CreateTriggerCollider();
  virtual IPhysicsCapsuleCharacterController *CreateCapsulseCharacterController();
  virtual IPhysicsJoint *CreateJoint(vkPhysicsJointType type, IPhysicsDynamicCollider *colliderA, IPhysicsDynamicCollider *colliderB);
  /** 
   * @}
   */

  /// don't use directly
  static vkEngine *static_instance;
private:

  IWindow *m_window;  
  IGraphics *m_renderer;
  IPhysicsSystem *m_physicsSystem;

};


VK_FORCEINLINE IWindow *vkEngine::GetWindow() const
{
  return m_window;
}

VK_FORCEINLINE IGraphics *vkEngine::GetRenderer() const
{
  return m_renderer;
}

VK_FORCEINLINE IPhysicsSystem *vkEngine::GetPhysicsSystem() const
{
  return m_physicsSystem;
}


#define vkEng (vkEngine::static_instance)