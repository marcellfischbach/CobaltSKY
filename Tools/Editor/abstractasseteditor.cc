

#include <abstractasseteditor.hh>
#include <assetmanager/assetmanagerassetwriter.hh>
#include <editor.hh>
#include <project/project.hh>
#include <mainwindow.hh>
#include <eventbus.hh>
#include <events/assetpreviewiconchangedevent.hh>
#include <events/assetrenamedevent.hh>
#include <cobalt/core/csfileinfo.hh>
#include <cobalt/core/csvfs.hh>
#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include <QMessageBox>

void abstract_asset_editor_asset_renamed(csEvent &event, void *userData)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);
  AbstractAssetEditor *editor = reinterpret_cast<AbstractAssetEditor*>(userData);
  editor->ResourceRenamed(evt.GetFrom(), evt.GetTo());
}


AbstractAssetEditor::AbstractAssetEditor()
  : iAssetEditor()
  , m_dirty(true)
  , m_editObject(0)
{
  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), abstract_asset_editor_asset_renamed, this);
}

AbstractAssetEditor::~AbstractAssetEditor()
{
  EventBus::Get().Deregister(abstract_asset_editor_asset_renamed, this);
}

void AbstractAssetEditor::SetObject(iObject *object, const AssetDescriptor &descriptor)
{
  CS_SET(m_editObject, object);
  m_assetDescriptor = descriptor;
  UpdateName();
  UpdateAsset();
}

void AbstractAssetEditor::UpdateName()
{

  csFileInfo info(m_assetDescriptor.GetLocator().GetResourceFile());
  m_name = info.GetName();

}

iObject *AbstractAssetEditor::GetEditObject()
{
  return m_editObject;
}

const iObject *AbstractAssetEditor::GetEditObject() const
{
  return m_editObject;
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

QString AbstractAssetEditor::GetResourceDataFileName() const
{
  const csResourceLocator &loc = GetAssetDescriptor().GetLocator().AsData();
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

void AbstractAssetEditor::ReplacePreviewIcon(QImage image)
{
  csResourceLocator dataLocator = m_assetDescriptor.GetLocator().AsData();

  printf("image: %s\n", dataLocator.GetDebugName().c_str());
  iFile *file = csVFS::Get()->Open(dataLocator, eOM_ReadWrite);

  AssetManagerAssetWriter writer;
  writer.Import(file);
  file->Close();
  writer.RemoveEntry("EDITOR_ICON");

  QBuffer buffer;
  QImageWriter qimagewriter(&buffer, QString("PNG").toLatin1());
  qimagewriter.write(image);

  writer.AddEntry(
    "EDITOR_ICON",
    csString("PNG"),
    buffer.data().length(),
    reinterpret_cast<const csUInt8*>(buffer.data().constData())
  );


  iFile *dataFile = csVFS::Get()->Open(dataLocator, eOM_Write);
  if (dataFile)
  {
    writer.Output(dataFile);
    dataFile->Close();
  }


  EventBus::Get() << AssetPreviewIconChangedEvent(m_assetDescriptor.GetLocator());
}

void AbstractAssetEditor::UpdateMainWindow()
{
  Editor::Get()->GetMainWindow()->UpdateEditor(this);
}

