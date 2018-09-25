#include <editor/importers/importerdialog.hh>
#include <editor/importers/iimporter.hh>
#include <editor/importers/importerfileslistmodel.hh>
#include <QGridLayout>
#include <iostream>
#include <editor/assetmanager/assetmanagerwidget.hh>


ImporterDialog::ImporterDialog(AssetManagerWidget *assetManagerWidget, Qt::WindowFlags f)
  : QDialog(assetManagerWidget, f)
  , m_activeImporter(0)
  , m_firstTimeShow(true)
  , m_assetManagerWidget(assetManagerWidget)
{
  setModal(false);
  setAttribute(Qt::WA_DeleteOnClose, true);

  m_gui.setupUi(this);

  m_model = new ImporterFilesListModel();
  m_gui.listView->setModel(m_model);
}


ImporterDialog::~ImporterDialog()
{
}

void ImporterDialog::showEvent(QShowEvent *evt)
{
  QDialog::showEvent(evt);

  if (m_firstTimeShow)
  {
    m_firstTimeShow = false;
    int left = 200;
    int right = width() - left;
    QList<int> sizes;
    sizes << left << right;
    m_gui.splitter->setSizes(sizes);
    m_gui.splitter->setStretchFactor(0, 0);
    m_gui.splitter->setStretchFactor(1, 1);
  }
}


void ImporterDialog::on_OK_clicked(bool)
{
  for (iImporter *importer : m_importers)
  {
    importer->Import(m_assetManagerWidget);
  }
  accept();
}

void ImporterDialog::on_Cancel_clicked (bool)
{
  reject();
}

void ImporterDialog::on_listView_clicked(const QModelIndex &index)
{
  m_activeImporter = m_importers[index.row()];
  m_gui.stackedWidget->setCurrentIndex(index.row());
  UpdateImporterName();
}


void ImporterDialog::AddImporter(iImporter *importer)
{
  m_importers.push_back(importer);
  m_model->add(importer->GetAssetName());
  m_gui.stackedWidget->addWidget(importer->GetWidget());
  if (!m_activeImporter)
  {
    m_activeImporter = importer;
    UpdateImporterName();
  }
}


void ImporterDialog::UpdateImporterName()
{
  if (m_activeImporter)
  {
    m_gui.label->setText(QString(m_activeImporter->GetAssetName().c_str()));
  }
}
