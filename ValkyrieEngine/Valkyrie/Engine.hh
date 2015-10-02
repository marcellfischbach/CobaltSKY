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

  int Run();

protected:
  void RegisterLoaders();

private:
  vkEngine();

  IWindow *m_window;  
  IRenderer *m_renderer;
};
