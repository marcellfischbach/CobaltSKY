

#include <stdio.h>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/Settings.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <SDLWindow/SDLWindow.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <PhysicsBullet/BulletSystem.hh>
#include <stdio.h>

int test(vkGraphicsGL4 *graphics)
{
  vkSGNode *defaultTxt = new vkSGNode(eSGNT_DefaultTextureCoord);

  vkSGNode *textureBias = new vkSGNode(eSGNT_VarFloat2);
  textureBias->SetBindingName("TextureBias");

  vkSGNode *add = new vkSGNode(eSGNT_AddFloat2);
  add->GetInput(0)->SetInput(defaultTxt->GetOutput(0));
  add->GetInput(1)->SetInput(textureBias->GetOutput(0));

  vkSGNode *sub = new vkSGNode(eSGNT_SubFloat2);
  sub->GetInput(0)->SetInput(add->GetOutput(0));
  sub->GetInput(1)->SetInput(textureBias->GetOutput(0));


  vkSGNode *texture = new vkSGNode(eSGNT_Texture2D);
  texture->SetBindingName("Diffuse");
  texture->GetInput("uv")->SetInput(sub->GetOutput(0));



  vkSGShaderGraph graph;
  graph.SetDiffuse(texture->GetOutput(0));
  graph.SetAlpha(texture->GetOutput(4));
  graph.SetDiscardAlpha(0.5f, eCM_Less);

  graphics->GetShaderGraphFactory()->GenerateShaderGraph(&graph);

  return 0;
}


int main(int argc, char **argv)
{

  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);
  vkEngine *engine = vkEngine::Get();

  // initialize the window
  SDLWindow *window = new SDLWindow();
  vkInt16 posX = 100;
  vkInt16 posY = 100;

#if 0
  posX = -1500;
#else
  //posX = 2000;
#endif
  if (!window->InitializeOpenGL("ValkyrieEngine Runner", 1366, 768, posX, posY, false, 4, 4))
  {
    delete window;
    return -1;
  }

  engine->SetWindow(window);

  vkGraphicsGL4 *GraphicsGL4 = new vkGraphicsGL4();
  engine->SetRenderer(GraphicsGL4);

  vkBulletSystem *bulletSystem = new vkBulletSystem();
  bulletSystem->Initialize();
  engine->SetPhysicsSystem(bulletSystem);

  test(GraphicsGL4);
  return 0;

  //return engine->Run();
}