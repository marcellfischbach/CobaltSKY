
#include <assetmanager/import/assetmanagerimporterdialog.hh>
#include <assetmanager/import/assetmanagerimporter.hh>
#include <assetmanager/import/assetmanagerimporterpage.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <editor.hh>
#include <events/assetaddedevent.hh>
#include <eventbus.hh>
#include <project/project.hh>
#include <project/projectreferencetree.hh>

AssetManagerImporterDialog::AssetManagerImporterDialog(AssetManagerWidget *assetManager)
  : QDialog(assetManager)
  , m_assetManager(assetManager)
{
  setModal(true);
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

void AssetManagerImporterDialog::AddImportData(AssetManagerImportData *data)
{
  if (!data)
  {
    return;
  }

  for (auto page : m_pages)
  {
    if (page->GetData() == data)
    {
      return;
    }
  }

  AssetManagerImporterPage *page = new AssetManagerImporterPage();
  connect(page, SIGNAL(Changed(AssetManagerImporterPage*)), this, SLOT(PageChanged(AssetManagerImporterPage*)));
  page->Setup(data);

  m_pages.push_back(page);

  m_gui.stackedWidget->addWidget(page);
  m_model.AddPage(page);

  QModelIndex idx = m_model.index(0, 0);
  m_gui.listView->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
  m_gui.stackedWidget->setCurrentIndex(0);
}

void AssetManagerImporterDialog::AddImportDatas(const std::vector<AssetManagerImportData*> &datas)
{
  for (auto data : datas)
  {
    AddImportData(data);
  }
}

void AssetManagerImporterDialog::on_pbOK_clicked(bool)
{
  EventBus &bus = EventBus::Get();
  for (auto page : m_pages)
  {
    if (page->IsImporting())
    {
      AssetManagerImportData *data = page->GetData();
      csResourceLocator loc = data->Import(m_assetManager);
      Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(loc);
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

