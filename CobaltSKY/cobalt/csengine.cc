

#include <cobalt/csengine.hh>
#include <cobalt/csmodule.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csloaders.hh>

csEngine *csEngine::static_instance = 0;

csEngine::csEngine()
  : m_window(0)
  , m_renderer(0)
  , m_physicsSystem(0)
{
  csEngine::static_instance = this;

  csCobaltSKYModule::Initialize();
  csLoaders::Register(csResourceManager::Get());
}


void csEngine::SetWindow(iWindow *window)
{
  CS_SET(m_window, window);
}

void csEngine::SetRenderer(iGraphics *renderer)
{
  CS_SET(m_renderer, renderer);
}

void csEngine::SetPhysicsSystem(iPhysicsSystem *physicsSystem)
{
  CS_SET(m_physicsSystem, physicsSystem);
}



iIndexBuffer *csEngine::CreateIndexBuffer(csSize size, const void *data, csBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateIndexBuffer(size, data, mode);
  }
  return 0;
}


iVertexBuffer *csEngine::CreateVertexBuffer(csSize size, const void *data, csBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexBuffer(size, data, mode);
  }
  return 0;
}

iVertexDeclaration *csEngine::CreateVertexDeclaration(const csVertexElement *elements) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexDeclaration(elements);
  }
  return 0;
}

iRenderTarget *csEngine::CreateRenderTarget() 
{
  if (m_renderer)
  {
    return m_renderer->CreateRenderTarget();
  }
  return 0;
}

iSampler *csEngine::CreateSampler()
{
  if (m_renderer)
  {
    return m_renderer->CreateSampler();
  }
  return 0;
}

iTexture2D *csEngine::CreateTexture2D(csPixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps)
{ 
  if (m_renderer)
  {
    return m_renderer->CreateTexture2D(format, width, height, mipmaps);
  }
  return 0;
}

iTexture2DArray *csEngine::CreateTexture2DArray(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTexture2DArray(format, width, height, layers, mipmaps);
  }
  return 0;
}

iTextureCube *csEngine::CreateTextureCube(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTextureCube(format, width, height, depth);
  }
  return 0;
}

iShader *csEngine::CreateShader(const csString &vertexCode, const csString &tessCtrl, const csString &tessEval, const csString &geometry, const csString &fragmentCode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateShader(vertexCode, tessCtrl, tessEval, geometry, fragmentCode);
  }
  return 0;
}





iPhysicsScene *csEngine::CreateScene()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateScene();
  }
  return 0;
}

iPhysicsShape *csEngine::CreateShape(const csPhysGeometry &geometry)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateShape(geometry);
  }
  return 0;
}

iPhysicsDynamicCollider *csEngine::CreateDynamicCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateDynamicCollider();
  }
  return 0;
}

iPhysicsStaticCollider *csEngine::CreateStaticCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateStaticCollider();
  }
  return 0;
}

iPhysicsTriggerCollider *csEngine::CreateTriggerCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateTriggerCollider();
  }
  return 0;
}

iPhysicsCapsuleCharacterController *csEngine::CreateCapsulseCharacterController()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateCapsulseCharacterController();
  }
  return 0;
}

iPhysicsJoint *csEngine::CreateJoint(csPhysicsJointType type, iPhysicsDynamicCollider *colliderA, iPhysicsDynamicCollider *colliderB)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateJoint(type, colliderA, colliderB);
  }
  return 0;
}
