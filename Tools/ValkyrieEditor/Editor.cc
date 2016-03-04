
#include <Editor.hh>
#include <EditorWindow.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/Settings.hh>
#include <Valkyrie/Core/VFS.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <PhysicsBullet/BulletSystem.hh>

Editor::Editor()
  : QObject()
{

}

bool Editor::Initialize(int argc, char **argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);

  m_editorWindow = new EditorWindow();
  m_editorWindow->resize(1024, 768);
  m_editorWindow->setVisible(true);


  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);
  m_engine = vkEngine::Get();

  m_graphics = new vkGraphicsGL4();
  m_engine->SetRenderer(m_graphics);

  m_physicsSystem = new vkBulletSystem();
  m_physicsSystem->Initialize();
  m_engine->SetPhysicsSystem(m_physicsSystem);


  return true;
}


Editor &Editor::Get()
{
  static Editor editor;
  return editor;
}