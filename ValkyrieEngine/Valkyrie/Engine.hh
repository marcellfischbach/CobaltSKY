#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Physics/IPhysicsSystem.hh>
#include <Valkyrie/Window/IWindow.hh>


class VKE_API vkEngine
{
public:
  static vkEngine *Get();

  void SetWindow(IWindow *window);
  void SetRenderer(IGraphics *renderer);
  void SetPhysicsSystem(IPhysicsSystem *physicsSystem);

  IWindow *GetWindow() const;
  IGraphics *GetRenderer() const;
  IPhysicsSystem *GetPhysicsSystem() const;

  bool Initialize();
  bool SetupTestScene();


private:
  vkEngine();

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
