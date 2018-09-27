
#include <editor/assetmanager/actions/import/assetmanagerimporterdialog.hh>
#include <editor/assetmanager/actions/import/assetmanagerimporterpage.hh>
#include <editor/iassetimporter.hh>
#include <editor/assetmanager/assetmanagerwidget.hh>
#include <editor/editor.hh>
#include <editor/events/assetaddedevent.hh>
#include <editor/eventbus.hh>
#include <editor/project/project.hh>

AssetManagerImporterDialog::AssetManagerImporterDialog(AssetManagerWidget *assetManager, const asset::model::Folder *folder)
  : QDialog(assetManager)
  , m_assetManager(assetManager)
  , m_folder(folder)
{
  setModal(false);
  m_gui.setupUi(this);
  m_gui.listView->setModel(&m_model);

  QRect size = Editor::Get()->GetScreenSize();
  int width = size.width();
  int listSize = 200;

  m_gui.splitter->setSizes(QList<int>() << listSize << width - listSize);
  m_gui.splitter->setStretchFactor(0, 0);
  m_gui.splitter->setStretchFactor(1, 1);

}


AssetManagerImporterDialog::~AssetManagerImporterDialog()
{
  for (auto page : m_pages)
  {
    delete page;
  }
  m_pages.clear();
}

void AssetManagerImporterDialog::on_listView_activated(const QModelIndex &index)
{
  m_gui.stackedWidget->setCurrentIndex(index.row());
}

void AssetManagerImporterDialog::on_listView_clicked(const QModelIndex &index)
{
  m_gui.stackedWidget->setCurrentIndex(index.row());
}

void AssetManagerImporterDialog::AddImporter(iAssetImporter *importer)
{
  if (!importer)
  {
    return;
  }

  for (auto page : m_pages)
  {
    if (page->GetImporter() == importer)
    {
      return;
    }
  }

  AssetManagerImporterPage *page = new AssetManagerImporterPage();
  connect(page, SIGNAL(Changed(AssetManagerImporterPage*)), this, SLOT(PageChanged(AssetManagerImporterPage*)));
  page->Setup(importer);

  m_pages.push_back(page);

  m_gui.stackedWidget->addWidget(page);
  m_model.AddPage(page);

  QModelIndex idx = m_model.index(0, 0);
  m_gui.listView->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
  m_gui.stackedWidget->setCurrentIndex(0);
}

void AssetManagerImporterDialog::AddImporter(const std::vector<iAssetImporter*> &importers)
{
  for (auto importer : importers)
  {
    AddImporter(importer);
  }
}

void AssetManagerImporterDialog::on_pbOK_clicked(bool)
{
  EventBus &bus = EventBus::Get();
  for (auto page : m_pages)
  {
    if (page->IsImporting())
    {
      iAssetImporter *importer = page->GetImporter();
      csResourceLocator loc = importer->Import(m_folder);
      AssetAddedEvent evt(loc, 0);
      bus << evt;
    }
  }
  accept();
}

void AssetManagerImporterDialog::on_pbCancel_clicked(bool)
{
  reject();
}

void AssetManagerImporterDialog::PageChanged(AssetManagerImporterPage *page)
{
  m_model.PageChanged(page);
}

