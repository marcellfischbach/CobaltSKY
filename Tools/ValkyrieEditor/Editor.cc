
#include <Editor.hh>
#include <EditorWindow.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/Settings.hh>
#include <Valkyrie/Core/VFS.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <PhysicsBullet/BulletSystem.hh>

#include <qapplication.h>

Editor::Editor()
  : QObject()
  , m_graphics(0)
  , m_physicsSystem(0)
  , m_engine(0)
{
  m_titleName = "ValkyrieEditor v1.0";
}

bool Editor::Initialize(int argc, char **argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);

  vkSettings::Initialize(argc, argv);
  vkVFS::Get()->Initialize(argc, argv);
  m_engine = vkEngine::Get();

  InitializeImporters();
  InitializeLoaders();
  //InitializeStyleSheet();

  m_editorWindow = new EditorWindow();
  m_editorWindow->resize(1024, 768);
  m_editorWindow->setVisible(true);
  //m_editorWindow->move(QPoint(-1500, 100));


  return true;
}


void Editor::RequestGraphics()
{
  if (m_graphics)
  {
    return;
  }

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


void Editor::InitializeStyleSheet()
{
  IFile *qss = vkVFS::Get()->Open("editor/dark.qss", eOM_Read, eTM_Text);
  if (!qss)
  {
    return;
  }

  vkSize length = qss->GetLength();
  char *buffer = new char[length + 1];
  qss->Read(buffer, length);
  buffer[length] = '\0';
  qss->Close();
  qss->Release();

  QString style(buffer);
  qApp->setStyleSheet(style);
  delete[] buffer;


}

#include <Mesh/MeshImporter.hh>
#include <Texture/TextureImporter.hh>
void Editor::InitializeImporters()
{
  assetmanager::ImporterRegistry *impReg = assetmanager::ImporterRegistry::Get();
  impReg->RegisterImporter(mesh::Importer::Get());
  impReg->RegisterImporter(texture::Importer::Get());

}

#include <AssetManager/EditorIconLoader.hh>
#include <ShaderGraph/MetaDataLoader.hh>


void Editor::InitializeLoaders()
{
  vkResourceManager *mgr = vkResourceManager::Get();
  mgr->RegisterLoader(new EditorIconAssetXMLLoader());
  mgr->RegisterLoader(new ShaderGraphMetaDataAssetXMLLoader());
}
