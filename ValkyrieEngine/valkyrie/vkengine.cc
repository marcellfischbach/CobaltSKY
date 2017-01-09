

#include <valkyrie/vkengine.hh>
#include <valkyrie/vkmodule.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/loaders/vkloaders.hh>

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


void vkEngine::SetWindow(iWindow *window)
{
  VK_SET(m_window, window);
}

void vkEngine::SetRenderer(iGraphics *renderer)
{
  VK_SET(m_renderer, renderer);
}

void vkEngine::SetPhysicsSystem(iPhysicsSystem *physicsSystem)
{
  VK_SET(m_physicsSystem, physicsSystem);
}



iIndexBuffer *vkEngine::CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateIndexBuffer(size, data, mode);
  }
  return 0;
}


iVertexBuffer *vkEngine::CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexBuffer(size, data, mode);
  }
  return 0;
}

iVertexDeclaration *vkEngine::CreateVertexDeclaration(const vkVertexElement *elements) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexDeclaration(elements);
  }
  return 0;
}

iRenderTarget *vkEngine::CreateRenderTarget() 
{
  if (m_renderer)
  {
    return m_renderer->CreateRenderTarget();
  }
  return 0;
}

iSampler *vkEngine::CreateSampler()
{
  if (m_renderer)
  {
    return m_renderer->CreateSampler();
  }
  return 0;
}

iTexture2D *vkEngine::CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps)
{ 
  if (m_renderer)
  {
    return m_renderer->CreateTexture2D(format, width, height, mipmaps);
  }
  return 0;
}

iTexture2DArray *vkEngine::CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTexture2DArray(format, width, height, layers, mipmaps);
  }
  return 0;
}

iTextureCube *vkEngine::CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTextureCube(format, width, height, depth);
  }
  return 0;
}

iShader *vkEngine::CreateShader(const vkString &vertexCode, const vkString &tessCtrl, const vkString &tessEval, const vkString &geometry, const vkString &fragmentCode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateShader(vertexCode, tessCtrl, tessEval, geometry, fragmentCode);
  }
  return 0;
}





iPhysicsScene *vkEngine::CreateScene()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateScene();
  }
  return 0;
}

iPhysicsShape *vkEngine::CreateShape(const vkPhysGeometry &geometry)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateShape(geometry);
  }
  return 0;
}

iPhysicsDynamicCollider *vkEngine::CreateDynamicCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateDynamicCollider();
  }
  return 0;
}

iPhysicsStaticCollider *vkEngine::CreateStaticCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateStaticCollider();
  }
  return 0;
}

iPhysicsTriggerCollider *vkEngine::CreateTriggerCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateTriggerCollider();
  }
  return 0;
}

iPhysicsCapsuleCharacterController *vkEngine::CreateCapsulseCharacterController()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateCapsulseCharacterController();
  }
  return 0;
}

iPhysicsJoint *vkEngine::CreateJoint(vkPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateJoint(type, colliderA, colliderB);
  }
  return 0;
}
