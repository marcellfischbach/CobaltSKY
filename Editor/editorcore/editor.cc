

#include <editorcore/editor.hh>
#include <editorcore/editorwindow.hh>
#include <editorcore/abstracteditor.hh>
#include <editorcore/ieditorfactory.hh>

#include <editormodel/model.hh>
#include <editormodel/pathscanner.hh>

#include <cobalt/core/cssettings.hh>
#include <cobalt/core/csvfs.hh>

#include <iostream>

namespace cs::editor::core
{


Editor::Editor()
  : m_editorWindow(nullptr)
  , m_model(nullptr)
{
  m_editorFactories.clear();
}

Editor* Editor::Get()
{
  static Editor static_editor;

  return &static_editor;
}


bool Editor::Initialize(int argc, char ** argv)
{
  for (int i = 1; i < argc; ++i)
  {
    std::string projectPath = std::string(argv[i]);
    if (!OpenProject(projectPath))
    {
      return false;
    }
  }

  if (!m_model)
  {
    OpenProject("D:/DEV/temp/CobaltSKYModelTest-Ref");
  }

  m_editorWindow = new EditorWindow();
  m_editorWindow->setVisible(true);
  return true;
}

bool Editor::OpenProject(const std::string& projectPath)
{
  cs::Settings::Get()->Initialize(projectPath + "/config.csf");
  cs::VFS::Get()->Initialize(cs::Settings::Get());

  cs::VFS* vfs = cs::VFS::Get();

  m_model = new cs::editor::model::Model();
  cs::editor::model::PathScanner ps(m_model);



  for (size_t i = 0, in = cs::VFS::Get()->GetNumberOfEntries(); i < in; ++i)
  {
    cs::editor::model::VFSEntryNode* entryNode = m_model->CreateVFSEntryNode();
    entryNode->SetEntry(&cs::VFS::Get()->GetEntry(i));

    cs::editor::model::Transaction tx = m_model->CreateTransaction();
    tx.Begin();
    m_model->Add(entryNode, m_model->GetRoot(), tx);
    ps.Scan(entryNode, tx);
    tx.Commit();

  }

  m_model->Debug();

  return true;
}

void Editor::OpenEditor(cs::editor::model::AssetNode* assetNode)
{
  if (m_editorWindow->ShowOpenEditor(assetNode))
  {
    return;
  }
  AbstractEditor* editor = nullptr;
  for (auto editorFactory : m_editorFactories)
  {
    if (editorFactory->CanEdit(assetNode))
    {
      editor = editorFactory->CreateEditor(assetNode);
      if (editor)
      {
        break;
      }
    }
  }
  if (!editor)
  {
    // TODO: Show error message box
    std::cout << "Unable to open editor for [" << assetNode->GetResourceLocator().Encode() << "]\n";
    return;
  }

  if (!m_editorWindow->ShowEditor(editor))
  {
    delete editor;
  }
}

void Editor::Register(iEditorFactory* editorFactory)
{
  m_editorFactories.push_back(editorFactory);
}

EditorWindow* Editor::GetEditorWindow()
{
  return m_editorWindow;
}


cs::editor::model::Model* Editor::GetModel()
{
  return m_model;
}

std::string Editor::GetWindowTitle() const
{
  return "CobaltSKY Editor";
}

}