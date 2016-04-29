

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Module.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Loaders/Loaders.hh>


vkEngine::vkEngine()
  : m_window(0)
  , m_renderer(0)
  , m_physicsSystem(0)
{
  vkValkyrieModule::Initialize();
  vkLoaders::Register(vkResourceManager::Get());
}

vkEngine *vkEngine::Get()
{
  static vkEngine engine;
  return &engine;
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

