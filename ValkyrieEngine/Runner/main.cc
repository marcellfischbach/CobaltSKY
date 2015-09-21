

#include <stdio.h>
#include <Valkyrie/Engine.hh>
#include <SDLWindow/SDLWindow.hh>
#include <RenderGL4/RendererGL4.hh>

int main(int argc, char **argv)
{
  vkEngine *engine = vkEngine::Get();

  // initialize the window
  SDLWindow *window = new SDLWindow();
  if (!window->InitializeOpenGL("ValkyrieEngine Runner", 1366, 768, 100, 100, false, 4, 4))
  {
    delete window;
    return -1;
  }

  engine->SetWindow(window);

  RendererGL4 *renderGL4 = new RendererGL4();
  engine->SetRenderer(renderGL4);



  return engine->Run();
}