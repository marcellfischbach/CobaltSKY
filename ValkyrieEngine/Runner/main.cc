

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
  vkSGNode *constFloat1 = new vkSGNode(eSGNT_ConstFloat3);
  constFloat1->GetInput(0)->SetConstData(10.0f);
  constFloat1->GetInput(1)->SetConstData(12.0f);
  constFloat1->GetInput(2)->SetConstData(13.0f);



  vkSGNode *constFloat2 = new vkSGNode(eSGNT_ConstFloat3);
  constFloat2->GetInput(0)->SetConstData(25.2f);
  constFloat2->GetInput(1)->SetConstData(27.0f);
  constFloat2->GetInput(2)->SetConstData(30.0f);


  vkSGNode *addFloat = new vkSGNode(eSGNT_AddFloat3);
  addFloat->GetInput("a")->SetInput(constFloat1->GetOutput(0));
  addFloat->GetInput("b")->SetInput(constFloat2->GetOutput(0));

  vkSGNode *add2Float = new vkSGNode(eSGNT_AddFloat3);
  add2Float->GetInput("a")->SetInput(addFloat->GetOutput(0));
  add2Float->GetInput("b")->SetInput(addFloat->GetOutput(0));

  vkSGNode *splitFloat = new vkSGNode(eSGNT_SplitFloat3);
  splitFloat->GetInput("v")->SetInput(add2Float->GetOutput(0));

  vkSGNode *split2Float = new vkSGNode(eSGNT_SplitFloat3);
  split2Float->GetInput("v")->SetInput(addFloat->GetOutput(0));

  vkSGNode *float3 = new vkSGNode(eSGNT_Float3);
  float3->GetInput("x")->SetInput(splitFloat->GetOutput(0));
  float3->GetInput("y")->SetInput(splitFloat->GetOutput(2));
  float3->GetInput("z")->SetInput(splitFloat->GetOutput(1));


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