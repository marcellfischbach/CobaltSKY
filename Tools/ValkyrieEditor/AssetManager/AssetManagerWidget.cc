

#include <AssetManager/AssetManagerWidget.hh>
#include <AssetManager/FolderItemModel.hh>
#include <qfilesystemmodel.h>
#include <qfiledialog.h>
#include <qfileinfo.h>

namespace assetmanager
{



FolderTreeModel::FolderTreeModel()
  : QAbstractItemModel()
{
  m_rootDir = QDir("G:/ide/devs/valkyrieengine/data");
  Scan(0, m_rootDir);
}

FolderTreeModel *FolderTreeModel::Get()
{
  static FolderTreeModel model;
  return &model;
}



QModelIndex FolderTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    Entry *entry = static_cast<Entry*>(parent.internalPointer());
    if (row < entry->entries.size())
    {
      return createIndex(row, column, entry->entries[row]);
    }
  }
  else
  {
    if (row < m_entries.size())
    {
      return createIndex(row, column, m_entries[row]);
    }
  }
  return QModelIndex();
}

QModelIndex FolderTreeModel::parent(const QModelIndex &child) const
{
  if (child.isValid())
  {
    Entry *entry = static_cast<Entry*>(child.internalPointer());
    if (entry->parent)
    {
      return createIndex(GetIndexOfEntry(entry->parent), child.column(), entry->parent);
    }
  }
  return QModelIndex();
}

int FolderTreeModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
  {
    Entry *entry = static_cast<Entry*>(parent.internalPointer());
    return entry->entries.size();
  }
  return m_entries.size();
}

int FolderTreeModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant FolderTreeModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid())
  {
    Entry *entry = static_cast<Entry*>(index.internalPointer());
    if (role == Qt::DisplayRole && index.column() == 0)
    {
      return entry->name;
    }
  }
  return QVariant();
}


int FolderTreeModel::GetIndexOfEntry(Entry *entry) const
{
  if (entry->parent)
  {
    return entry->parent->entries.indexOf(entry);
  }
  return m_entries.indexOf(entry);
}


void FolderTreeModel::Scan(Entry *parent, const QDir &path)
{
  Entry *entry = new Entry();
  entry->name = path.dirName();
  entry->dir = path;
  entry->parent = parent;
  entry->entries.clear();

  if (parent)
  {
    parent->entries.append(entry);
  }
  else
  {
    m_entries.append(entry);
  }

  QStringList entries = path.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (QString pathEntry : entries)
  {
    QDir nextPath(path);
    if (nextPath.cd(pathEntry))
    {
      Scan(entry, nextPath);
    }
  }
}

QDir FolderTreeModel::GetDir(const QModelIndex &index) const
{
  if (index.isValid())
  {
    Entry *entry = static_cast<Entry*>(index.internalPointer());
    return entry->dir;
  }
  return QDir();
}

const QDir &FolderTreeModel::GetRootDir() const
{
  return m_rootDir;
}


AssetManagerWidget::AssetManagerWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);

  FolderTreeModel *folderModel = FolderTreeModel::Get();
  m_gui.tvFolders->setModel(folderModel);

  m_contentModel = new FolderItemModel();
  m_contentModel->SetPath(folderModel->GetRootDir());
  m_gui.lvContent->setModel(m_contentModel);
  

}

AssetManagerWidget::~AssetManagerWidget()
{

}



void AssetManagerWidget::on_pbImport_clicked(bool)
{
  printf("Import\n");

  QStringList files = QFileDialog::getOpenFileNames(this, tr("Select file to import..."), FolderTreeModel::Get()->GetRootDir().absolutePath());

  bool anySuccess = false;
  for (QString &file : files)
  {
    QFileInfo info(file);
    assetmanager::Importer *importer = assetmanager::ImporterRegistry::Get()->GetImporter(info);
    if (importer)
    {
      if (importer->Import(info, m_contentModel->GetPath()))
      {
        anySuccess = true;
      }
    }
  }
  if (anySuccess)
  {
    m_contentModel->Refresh();
  }
}

void AssetManagerWidget::on_tvFolders_clicked(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }
  QDir dir = FolderTreeModel::Get()->GetDir(index);
  m_contentModel->SetPath(dir);
}

void AssetManagerWidget::on_lvContent_doubleClicked(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  vkResourceLocator resourceLocator = m_contentModel->GetLocator(index);
  emit ResourceActivated(resourceLocator);
}

AssetManagerDialog::AssetManagerDialog(QWidget *parent)
  : QDialog(parent)
{
  setModal(false);

  m_widget = new AssetManagerWidget(this);

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setHorizontalSpacing(0);
  layout->setVerticalSpacing(0);
  layout->addWidget(m_widget);
}

AssetManagerDialog::~AssetManagerDialog()
{

}


}

