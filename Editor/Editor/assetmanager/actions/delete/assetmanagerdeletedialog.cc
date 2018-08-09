
#include <assetmanager/actions/delete/assetmanagerdeletedialog.hh>
#include <editor.hh>

AssetManagerDeleteDialog::AssetManagerDeleteDialog(QWidget *parent)
  : QDialog(parent)
  , m_deleteOverloaded(false)
  , m_deleteSuper(false)
{
  m_gui.setupUi(this);
  setModal(true);
  setWindowTitle(QString(Editor::Get()->GetApplicationTitle().c_str()));
}

AssetManagerDeleteDialog::~AssetManagerDeleteDialog()
{
  
}

void AssetManagerDeleteDialog::SetAssetName(const std::string &name)
{
  QString msg = tr("Delete asset '%1'?").arg(QString(name.c_str()));
  m_gui.message->setText(msg);
}

void AssetManagerDeleteDialog::on_pbOK_clicked()
{
  m_deleteOverloaded = m_gui.cbDeleteOverloaded->isChecked();
  m_deleteSuper = m_gui.cbDeleteSuper->isChecked();
  accept();
}

void AssetManagerDeleteDialog::on_pbCancel_clicked()
{
  reject();
}

