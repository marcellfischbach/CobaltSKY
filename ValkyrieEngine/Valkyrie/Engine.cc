

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Window/IKeyboard.hh>

vkEngine::vkEngine()
  : m_window (0)
  , m_renderer(0)
{

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

void vkEngine::SetRenderer(IRenderer *renderer)
{
  VK_SET(m_renderer, renderer);
}

int vkEngine::Run()
{
  if (!m_window)
  {
    return -1;
  }

  const IKeyboard *keyboard = m_window->GetKeyboard();

  while (true)
  {
    m_window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }

    m_window->Present();

  }

  return 0;
}