#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <Valkyrie/Window/IWindow.hh>

class VKE_API vkEngine
{
public:
  static vkEngine *Get();

  void SetWindow(IWindow *window);
  void SetRenderer(IRenderer *renderer);

  IWindow *GetWindow() const;
  IRenderer *GetRenderer() const;

  int Run();

protected:
  void RegisterLoaders();

private:
  vkEngine();

  IWindow *m_window;  
  IRenderer *m_renderer;
};


VK_FORCEINLINE IWindow *vkEngine::GetWindow() const
{
  return m_window;
}

VK_FORCEINLINE IRenderer *vkEngine::GetRenderer() const
{
  return m_renderer;
}