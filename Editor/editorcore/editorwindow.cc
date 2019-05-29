
#include <editorcore/editorwindow.hh>
#include <editorcore/editor.hh>
#include <editorcore/abstracteditor.hh>
#include <editorcore/itoolview.hh>

#include <QMessageBox>


namespace cs::editor::core
{


EditorWindow::EditorWindow()
  : QMainWindow()
{
  InitUI();

  resize(1024, 768);
}

void EditorWindow::InitUI()
{
  m_editorTabs = new QTabWidget();
  m_editorTabs->setMovable(true);
  m_editorTabs->setTabsClosable(true);

  connect(m_editorTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(OnTabCloseRequest(int)));
  setCentralWidget(m_editorTabs);
}


void EditorWindow::Add(iToolView* toolView)
{
  QDockWidget* dockWidget = new QDockWidget(toolView->GetTitle(), this);
  dockWidget->setAllowedAreas(toolView->GetAllowedAreas());
  dockWidget->setWidget(toolView->GetWidget());
  addDockWidget(toolView->GetDefaultArea(), dockWidget);
}

bool EditorWindow::ShowOpenEditor(cs::editor::model::AssetNode* asset)
{

  auto it = m_openEditors.find(asset);
  if (it == m_openEditors.end())
  {
    return false;
  }

  AbstractEditor* editor = it->second;
  if (!editor)
  {
    m_openEditors.erase(it);
    return false;
  }

  if (!ShowEditor(editor))
  {
    delete editor;
    m_openEditors.erase(it);
    return false;
  }

  return true;
}


bool EditorWindow::ShowEditor(AbstractEditor* editor)
{
  if (!editor || !editor->GetAsset())
  {
    return false;
  }

  QWidget* editorWidget = editor->GetWidget();
  if (!editorWidget)
  {
    return false;
  }

  int idx = m_editorTabs->indexOf(editorWidget);
  if (idx == -1)
  {
    idx = m_editorTabs->addTab(editorWidget, GetEditorTitle(editor));
  }

  m_editorTabs->setCurrentIndex(idx);

  m_openEditors[editor->GetAsset()] = editor;

  return true;
}

void EditorWindow::OnTabCloseRequest(int idx)
{
  QWidget* widget = m_editorTabs->widget(idx);

  if (!widget)
  {
    m_editorTabs->removeTab(idx);
    return;
  }

  AbstractEditor* editor = nullptr;

  for (auto openEditor : m_openEditors)
  {
    if (openEditor.second->GetWidget() == widget)
    {
      editor = openEditor.second;
      break;
    }
  }
  if (!editor)
  {
    m_editorTabs->removeTab(idx);
    return;
  }

  if (editor->HasPendingChanges())
  {
    int res = QMessageBox::question(this, QString(Editor::Get()->GetWindowTitle().c_str()),
      tr("There are pending changes. Closing the editor will discard the changes\nDo you want to close the editor?"),
      QMessageBox::Yes,
      QMessageBox::No);
    if (res == QMessageBox::No)
    {
      return;
    }
  }

  m_editorTabs->removeTab(idx);
  auto it = m_openEditors.find(editor->GetAsset());
  if (it != m_openEditors.end())
  {
    m_openEditors.erase(it);
  }
  delete editor;
  
}

QString EditorWindow::GetEditorTitle(AbstractEditor* editor)
{
  QString result = "";
  if (editor)
  {
    result = QString(editor->GetName().c_str());
    if (editor->HasPendingChanges())
    {
      result += QString(" *");
    }
  }
  return result;
}

}