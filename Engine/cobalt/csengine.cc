

#include <cobalt/csengine.hh>
#include <cobalt/csmodule.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/loaders/csloaders.hh>

cs::Engine *cs::Engine::static_instance = 0;

cs::Engine::Engine()
  : m_window(0)
  , m_renderer(0)
  , m_physicsSystem(0)
{
  cs::Engine::static_instance = this;

  cs::CobaltSKYModule::Initialize();
  cs::Loaders::Register(cs::ResourceManager::Get());
}


void cs::Engine::SetWindow(cs::iWindow *window)
{
  CS_SET(m_window, window);
}

void cs::Engine::SetRenderer(cs::iGraphics *renderer)
{
  CS_SET(m_renderer, renderer);
}

void cs::Engine::SetPhysicsSystem(cs::iPhysicsSystem *physicsSystem)
{
  CS_SET(m_physicsSystem, physicsSystem);
}



cs::iIndexBuffer *cs::Engine::CreateIndexBuffer(csSize size, const void *data, cs::eBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateIndexBuffer(size, data, mode);
  }
  return 0;
}


cs::iVertexBuffer *cs::Engine::CreateVertexBuffer(csSize size, const void *data, cs::eBufferDataMode mode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexBuffer(size, data, mode);
  }
  return 0;
}

cs::iVertexDeclaration *cs::Engine::CreateVertexDeclaration(const cs::VertexElement *elements) 
{
  if (m_renderer)
  {
    return m_renderer->CreateVertexDeclaration(elements);
  }
  return 0;
}

cs::iRenderTarget *cs::Engine::CreateRenderTarget() 
{
  if (m_renderer)
  {
    return m_renderer->CreateRenderTarget();
  }
  return 0;
}

cs::iSampler *cs::Engine::CreateSampler()
{
  if (m_renderer)
  {
    return m_renderer->CreateSampler();
  }
  return 0;
}

cs::iTexture2D *cs::Engine::CreateTexture2D(cs::ePixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps)
{ 
  if (m_renderer)
  {
    return m_renderer->CreateTexture2D(format, width, height, mipmaps);
  }
  return 0;
}

cs::iTexture2DArray *cs::Engine::CreateTexture2DArray(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTexture2DArray(format, width, height, layers, mipmaps);
  }
  return 0;
}

cs::iTextureCube *cs::Engine::CreateTextureCube(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth) 
{
  if (m_renderer)
  {
    return m_renderer->CreateTextureCube(format, width, height, depth);
  }
  return 0;
}

cs::iShader *cs::Engine::CreateShader(const std::string &vertexCode, const std::string &tessCtrl, const std::string &tessEval, const std::string &geometry, const std::string &fragmentCode) 
{
  if (m_renderer)
  {
    return m_renderer->CreateShader(vertexCode, tessCtrl, tessEval, geometry, fragmentCode);
  }
  return 0;
}





cs::iPhysicsScene *cs::Engine::CreateScene()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateScene();
  }
  return 0;
}

cs::iPhysicsShape *cs::Engine::CreateShape(const cs::PhysGeometry &geometry)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateShape(geometry);
  }
  return 0;
}

cs::iPhysicsDynamicCollider *cs::Engine::CreateDynamicCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateDynamicCollider();
  }
  return 0;
}

cs::iPhysicsStaticCollider *cs::Engine::CreateStaticCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateStaticCollider();
  }
  return 0;
}

cs::iPhysicsTriggerCollider *cs::Engine::CreateTriggerCollider()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateTriggerCollider();
  }
  return 0;
}

cs::iPhysicsCapsuleCharacterController *cs::Engine::CreateCapsulseCharacterController()
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateCapsulseCharacterController();
  }
  return 0;
}

cs::iPhysicsJoint *cs::Engine::CreateJoint(cs::ePhysicsJointType type, cs::iPhysicsDynamicCollider *colliderA, cs::iPhysicsDynamicCollider *colliderB)
{
  if (m_physicsSystem)
  {
    return m_physicsSystem->CreateJoint(type, colliderA, colliderB);
  }
  return 0;
}
