
#include <Editor.hh>
#include <EditorWindow.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/Settings.hh>
#include <Valkyrie/Core/VFS.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <PhysicsBullet/BulletSystem.hh>

Editor::Editor()
  : QObject()
  , m_graphics(0)
  , m_physicsSystem(0)
  , m_engine(0)
{

}

bool Editor::Initialize(int argc, char **argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);

  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);

  m_editorWindow = new EditorWindow();
  m_editorWindow->resize(1024, 768);
  m_editorWindow->setVisible(true);
  //m_editorWindow->move(QPoint(-1500, 100));


  return true;
}


void Editor::RequestGraphics()
{
  if (m_engine)
  {
    return;
  }

  m_engine = vkEngine::Get();

  m_graphics = new vkGraphicsGL4();
  m_engine->SetRenderer(m_graphics);

  m_physicsSystem = new vkBulletSystem();
  m_physicsSystem->Initialize();
  m_engine->SetPhysicsSystem(m_physicsSystem);


}

Editor &Editor::Get()
{
  static Editor editor;
  return editor;
}