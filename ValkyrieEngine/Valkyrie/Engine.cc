

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Module.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/Loaders.hh>

vkEngine *vkEngine::static_instance = 0;

vkEngine::vkEngine()
  : m_window(0)
  , m_renderer(0)
  , m_physicsSystem(0)
{
  vkEngine::static_instance = this;

  vkValkyrieModule::Initialize();
  vkLoaders::Register(vkResourceManager::Get());
}


void vkEngine::SetWindow(IWindow *window)
{
  VK_SET(m_window, window);
}

void vkEngine::SetRenderer(IGraphics *renderer)
{
  VK_SET(m_renderer, renderer);
}

void vkEngine::SetPhysicsSystem(IPhysicsSystem *physicsSystem)
{
  VK_SET(m_physicsSystem, physicsSystem);
}





IObject *vkEngine::Get(const vkResourceLocator &locator, bool forceInstance) const
{
  if (forceInstance)
  {
    return vkResourceManager::Get()->Load(locator, 0);
  }
  else
  {
    return vkResourceManager::Get()->GetOrLoad(locator, 0);
  }
}


IIndexBuffer *vkEngine::CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateIndexBuffer(size, data, mode);
  }
  return 0;
}


IVertexBuffer *vkEngine::CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexBuffer(size, data, mode);
  }
  return 0;
}

IVertexDeclaration *vkEngine::CreateVertexDeclaration(const vkVertexElement *elements) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexDeclaration(elements);
  }
  return 0;
}

IRenderTarget *vkEngine::CreateRenderTarget() 
{
  if (m_renderer)
  {
    return m_renderer->CreateRenderTarget();
  }
  return 0;
}

ISampler *vkEngine::CreateSampler()
{
  if (m_renderer)
  {
    return m_renderer->CreateSampler();
  }
  return 0;
}

ITexture2D *vkEngine::CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps)
{ 
  if (m_renderer)
  {
    return m_renderer->CreateTexture2D(format, width, height, mipmaps);
  }
  return 0;
}

ITexture2DArray *vkEngine::CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTexture2DArray(format, width, height, layers, mipmaps);
  }
  return 0;
}

ITextureCube *vkEngine::CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTextureCube(format, width, height, depth);
  }
  return 0;
}

IShader *vkEngine::CreateShader(const vkString &vertexCode, const vkString &tessCtrl, const vkString &tessEval, const vkString &geometry, const vkString &fragmentCode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateShader(vertexCode, tessCtrl, tessEval, geometry, fragmentCode);
  }
  return 0;
}





IPhysicsScene *vkEngine::CreateScene()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateScene();
  }
  return 0;
}

IPhysicsShape *vkEngine::CreateShape(const vkPhysGeometry &geometry)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateShape(geometry);
  }
  return 0;
}

IPhysicsDynamicCollider *vkEngine::CreateDynamicCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateDynamicCollider();
  }
  return 0;
}

IPhysicsStaticCollider *vkEngine::CreateStaticCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateStaticCollider();
  }
  return 0;
}

IPhysicsTriggerCollider *vkEngine::CreateTriggerCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateTriggerCollider();
  }
  return 0;
}

IPhysicsCapsuleCharacterController *vkEngine::CreateCapsulseCharacterController()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateCapsulseCharacterController();
  }
  return 0;
}

IPhysicsJoint *vkEngine::CreateJoint(vkPhysicsJointType type, IPhysicsDynamicCollider *colliderA, IPhysicsDynamicCollider *colliderB)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateJoint(type, colliderA, colliderB);
  }
  return 0;
}
