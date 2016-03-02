

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
  vkSGDefaultTextureCoordinate *defaultTxt = new vkSGDefaultTextureCoordinate();

  vkSGVarFloat2 *textureBias = new vkSGVarFloat2();
  textureBias->SetBindingName("TextureBias");

  vkSGConstFloat *fl = new vkSGConstFloat();
  fl->GetInput(0)->SetConst(10.0f);

  vkSGFloat2 *textureBias3 = new vkSGFloat2();
  textureBias3->GetInput(0)->SetConst(0.25f);
  textureBias3->SetInput(1, fl);
  //textureBias3->SetBindingName("TextureBias3");

  vkSGAdd *add = new vkSGAdd();
  add->SetInput(0, defaultTxt);
  add->SetInput(1, textureBias);

  vkSGSub *sub = new vkSGSub();
  sub->SetInput(0, add);
  sub->GetInput(1)->SetConst(0.5f);
  sub->SetInput(1, textureBias3);


  vkSGTexture2D *texture = new vkSGTexture2D();
  texture->SetBindingName("Diffuse");
  texture->SetInput("uv", sub);



  vkSGShaderGraph graph;
  graph.SetDiffuse(texture->GetOutput(0));
  graph.SetAlpha(texture->GetOutput(4));
  graph.SetDiscardAlpha(0.5f, eCM_Less);

  graphics->GetShaderGraphFactory()->GenerateShaderGraph(&graph);

  printf("Messages:\n");
  printf("defaultTxt: %s\n", defaultTxt->GetValidationMessage().c_str());
  printf("textureBias: %s\n", textureBias->GetValidationMessage().c_str());
  printf("textureBias3: %s\n", textureBias3->GetValidationMessage().c_str());
  printf("add: %s\n", add->GetValidationMessage().c_str());
  printf("sub: %s\n", sub->GetValidationMessage().c_str());
  printf("texture: %s\n", texture->GetValidationMessage().c_str());


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