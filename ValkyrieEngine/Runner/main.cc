

#include <stdio.h>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/Settings.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <SDLWindow/SDLWindow.hh>
#include <RenderGL4/RendererGL4.hh>

int main(int argc, char **argv)
{
  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);
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


  IShader *shader = vkResourceManager::Get()->Load<IShader>(vkResourceLocator("${shaders}/solid_gbuffer.xml", "GBuffer"));
  printf("Shader: %p\n", shader);

  return engine->Run();
}