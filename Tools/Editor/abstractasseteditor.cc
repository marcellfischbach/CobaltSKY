

#include <abstractasseteditor.hh>
#include <QMessageBox>
#include <editor.hh>
#include <project/project.hh>
#include <mainwindow.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <QFile>


AbstractAssetEditorObject::AbstractAssetEditorObject(AbstractAssetEditor *editor)
  : QObject()
  , m_editor(editor)
{
  Project *proj = Editor::Get()->GetProject();

  connect(proj, SIGNAL(ResourceRenamed(const csResourceLocator &, const csResourceLocator &)),
    this, SLOT(ResourceRenamed(const csResourceLocator &, const csResourceLocator &)));
}

void AbstractAssetEditorObject::ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to)
{
  m_editor->ResourceRenamed(from, to);
}

AbstractAssetEditor::AbstractAssetEditor()
  : iAssetEditor()
  , m_dirty(true)
  , m_object(new AbstractAssetEditorObject(this))
{
}

AbstractAssetEditor::~AbstractAssetEditor()
{
  m_object->deleteLater();
}

void AbstractAssetEditor::SetAssetDescriptor(const AssetDescriptor &descriptor)
{
  m_assetDescriptor = descriptor;
  UpdateName();
  OpenAsset();
}

void AbstractAssetEditor::UpdateName()
{

  csFileInfo info(m_assetDescriptor.GetLocator().GetResourceFile());
  m_name = info.GetName();

}

const AssetDescriptor &AbstractAssetEditor::GetAssetDescriptor() const
{
  return m_assetDescriptor;
}



void AbstractAssetEditor::SetWidget(QWidget *widget)
{
  m_widget = widget;
}

QWidget *AbstractAssetEditor::GetWidget()
{
  return m_widget;
}

const csString &AbstractAssetEditor::GetName() const
{
  return m_name;
}

void AbstractAssetEditor::SetDirty(bool dirty)
{
  if (m_dirty != dirty)
  {
    m_dirty = dirty;
    UpdateMainWindow();
  }
}

bool AbstractAssetEditor::IsDirty() const
{
  return m_dirty;
}

void AbstractAssetEditor::AddDockItemName(const csString &dockItemName)
{
  m_visibleDockItems.insert(dockItemName);
}

const std::set<csString> &AbstractAssetEditor::GetVisibleDockItems() const
{
  return m_visibleDockItems;
}

void AbstractAssetEditor::PopulateDockItems()
{
  // don't do anything... override in subclasses
}

void AbstractAssetEditor::CloseRequest()
{
  if (m_dirty)
  {
    int res = QMessageBox::question(Editor::Get()->GetMainWindow(),
      QObject::tr("CobaltSKY Editor"),
      QObject::tr("Close without saving?"),
      QMessageBox::Yes,
      QMessageBox::Cancel);
    if (res != QMessageBox::Yes)
    {
      return;
    }
  }

  Editor::Get()->GetMainWindow()->CloseEditor(this, true);
}


QString AbstractAssetEditor::GetResourceFileName() const
{
  const csResourceLocator &loc = GetAssetDescriptor().GetLocator();
  csString absFileName = csVFS::Get()->GetAbsolutePath(loc.GetResourceFile(), loc.GetResourceEntry());
  QString fileName(absFileName.c_str());
  return fileName;
}

void AbstractAssetEditor::ResourceRenamed(const csResourceLocator &from, const csResourceLocator &to)
{
  if (m_assetDescriptor.Renamed(from, to))
  {
    UpdateName();
    UpdateMainWindow();
  }
}

void AbstractAssetEditor::UpdateMainWindow()
{
  Editor::Get()->GetMainWindow()->UpdateEditor(this);
}

