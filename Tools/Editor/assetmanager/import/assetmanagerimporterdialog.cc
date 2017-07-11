
#include <assetmanager/import/assetmanagerimporterdialog.hh>
#include <assetmanager/import/assetmanagerimporter.hh>
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
  for (AssetManagerImportData *data : m_datas)
  {
    delete data;
  }
  m_datas.clear();
}

void AssetManagerImporterDialog::on_listView_activated(const QModelIndex &index)
{
  m_gui.stackedWidget->setCurrentIndex(index.row());
}

void AssetManagerImporterDialog::AddImportData(AssetManagerImportData *data)
{
  if (!data)
  {
    return;
  }

  m_datas.push_back(data);
  QWidget *widget = data->GetWidget();

  m_gui.stackedWidget->addWidget(widget);
  m_model.AddData(data);

  QModelIndex idx = m_model.index(0, 0);
  m_gui.listView->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
  m_gui.stackedWidget->setCurrentIndex(0);
}

void AssetManagerImporterDialog::on_pbOK_clicked(bool)
{
  EventBus &bus = EventBus::Get();
  for (AssetManagerImportData *data : m_datas)
  {
    csResourceLocator loc = data->Import(m_assetManager);
    bus << AssetAddedEvent(loc, 0);
  }
  accept();
}

void AssetManagerImporterDialog::on_pbCancel_clicked(bool)
{
  reject();
}

