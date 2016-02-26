

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

int main(int argc, char **argv)
{
  vkSGConstFloat3 *constFloat1 = new vkSGConstFloat3();
  constFloat1->SetValue(10.0f, 12.0f, 13.0f);


  vkSGConstFloat3 *constFloat2 = new vkSGConstFloat3();
  constFloat2->SetValue(25.2f, 27.0f, 30.0f);


  vkSGAddFloat3 *addFloat = new vkSGAddFloat3();
  addFloat->GetInput("A")->SetOutput(constFloat1->GetOutput(0));
  addFloat->GetInput("B")->SetOutput(constFloat2->GetOutput(0));

  vkSGAddFloat3 *add2Float = new vkSGAddFloat3();
  add2Float->GetInput("A")->SetOutput(addFloat->GetOutput(0));
  add2Float->GetInput("B")->SetOutput(addFloat->GetOutput(0));

  vkSGSplitFloat3 *splitFloat = new vkSGSplitFloat3();
  splitFloat->GetInput("Val")->SetOutput(add2Float->GetOutput(0));

  vkSGSplitFloat3 *split2Float = new vkSGSplitFloat3();
  split2Float->GetInput("Val")->SetOutput(addFloat->GetOutput(0));

  vkSGFloat3 *float3 = new vkSGFloat3();
  float3->GetInput("x")->SetOutput(splitFloat->GetOutput(0));
  float3->GetInput("y")->SetOutput(split2Float->GetOutput(2));
  float3->GetInput("z")->SetOutput(splitFloat->GetOutput(1));


  printf("ConstFloat1: %p\n", constFloat1);
  printf("ConstFloat2: %p\n", constFloat2);
  printf("AddFloat   : %p\n", addFloat);
  printf("AddFloat2  : %p\n", add2Float);
  printf("Split      : %p\n", splitFloat);
  printf("Float3      : %p\n", float3);

  vkShaderGraphGL4 graph;
  vkString code = graph.CreateCode(float3, 0);
  printf("Code:\n%s\n", code.c_str());

  return 0;
}


int main1(int argc, char **argv)
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


  return engine->Run();
}