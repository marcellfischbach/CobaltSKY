

#include <AssetManager/AssetManagerWidget.hh>
#include <AssetManager/FolderItemModel.hh>
#include <AssetManager/FolderTreeModel.hh>
#include <qfilesystemmodel.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qmenu.h>

namespace assetmanager
{



AssetManagerWidget::AssetManagerWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);

  FolderTreeModel *folderModel = FolderTreeModel::Get();
  m_gui.tvFolders->setModel(folderModel);

  m_contentModel = new FolderItemModel();
  m_contentModel->SetPath(folderModel->GetRootDir());
  m_gui.lvContent->setModel(m_contentModel);
  
  m_gui.lvContent->setContextMenuPolicy(Qt::CustomContextMenu);
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

void AssetManagerWidget::on_lvContent_customContextMenuRequested(const QPoint& p)
{
  QMenu menu(this);
  QModelIndex index = m_gui.lvContent->indexAt(p);
  if (!index.isValid())
  {
    return;
  }

  if (m_contentModel->IsContainer(index))
  {
    if (m_contentModel->IsContainerOpen(index))
    {
      QAction *closeContainerAction = menu.addAction(tr("Close container"));
      connect(closeContainerAction, SIGNAL(triggered(bool)), this, SLOT(CloseSelectedContainer(bool)));
    }
    else
    {
      QAction *openContainerAction = menu.addAction(tr("Open container"));
      connect(openContainerAction, SIGNAL(triggered(bool)), this, SLOT(OpenSelectedContainer(bool)));
    }

  }
  menu.exec(m_gui.lvContent->mapToGlobal(p));
}

void AssetManagerWidget::OpenSelectedContainer(bool)
{
  QModelIndex index = m_gui.lvContent->selectionModel()->currentIndex();
  m_contentModel->OpenContainer(index);
}

void AssetManagerWidget::CloseSelectedContainer(bool)
{
  QModelIndex index = m_gui.lvContent->selectionModel()->currentIndex();
  m_contentModel->CloseContainer(index);
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

