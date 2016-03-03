

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


  vkSGTexture2D *texture = new vkSGTexture2D();
  texture->SetBindingName("Diffuse");




  vkSGConstFloat3 *const1 = new vkSGConstFloat3();
  const1->SetInput(0, 0.25f);
  const1->SetInput(1, 0.75f);

  vkSGConstFloat3 *const2 = new vkSGConstFloat3();
  const2->SetInput(0, 1.25f);
  const2->SetInput(1, 1.75f);

  vkSGAdd *add12 = new vkSGAdd();
  add12->SetInput(0, const1);
  add12->SetInput(1, const2);

  vkSGSplitFloat3 *split3 = new vkSGSplitFloat3();
  split3->SetInput(0, add12);


  vkSGFloat3 *newFloat3 = new vkSGFloat3();
  newFloat3->SetInput(0, split3, 0);
  newFloat3->SetInput(1, split3, 2);
  newFloat3->SetInput(2, split3, 1);


  vkSGSplitFloat3 *split3a = new vkSGSplitFloat3();
  split3a->SetInput(0, newFloat3);

  vkSGFloat2 *float2 = new vkSGFloat2();
  float2->SetInput(0, split3a, 0);
  float2->SetInput(1, split3a, 1);

  vkSGTexture2D *alphaBlend = new vkSGTexture2D();
  alphaBlend->SetBindingName("AlphaBlend");
  alphaBlend->SetInput("uv", float2);



  vkSGShaderGraph graph;
  graph.SetDiffuse(newFloat3->GetOutput(0));
  graph.SetRoughness(split3->GetOutput(0));
  graph.SetAlpha(alphaBlend->GetOutput(4));
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