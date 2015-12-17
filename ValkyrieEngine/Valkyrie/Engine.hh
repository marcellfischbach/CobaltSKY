#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Window/IWindow.hh>

class VKE_API vkEngine
{
public:
  static vkEngine *Get();

  void SetWindow(IWindow *window);
  void SetRenderer(IGraphics *renderer);

  IWindow *GetWindow() const;
  IGraphics *GetRenderer() const;

  int Run();

protected:
  void RegisterLoaders();
  void RegisterClasses();

private:
  vkEngine();

  IWindow *m_window;  
  IGraphics *m_renderer;
};


VK_FORCEINLINE IWindow *vkEngine::GetWindow() const
{
  return m_window;
}

VK_FORCEINLINE IGraphics *vkEngine::GetRenderer() const
{
  return m_renderer;
}