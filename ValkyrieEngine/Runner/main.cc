

#include <stdio.h>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/Settings.hh>
#include <Valkyrie/Core/VFS.hh>
#include <SDLWindow/SDLWindow.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <stdio.h>

int main(int argc, char **argv)
{

  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);
  vkEngine *engine = vkEngine::Get();

  // initialize the window
  SDLWindow *window = new SDLWindow();
  vkInt16 posX = 100;
  vkInt16 posY = 100;

  //posX = -1500;
  if (!window->InitializeOpenGL("ValkyrieEngine Runner", 1366, 768, posX, posY, false, 4, 4))
  {
    delete window;
    return -1;
  }

  engine->SetWindow(window);

  vkGraphicsGL4 *GraphicsGL4 = new vkGraphicsGL4();
  engine->SetRenderer(GraphicsGL4);


  return engine->Run();
}