

#include <assetmanager/actions/rename/assetmanagerrenamedialog.hh>

AssetManagerRenameDialog::AssetManagerRenameDialog(QWidget *parent)
  : QDialog(parent)
{
  m_gui.setupUi(this);
}

AssetManagerRenameDialog::~AssetManagerRenameDialog()
{

}


void AssetManagerRenameDialog::SetName(const QString &name)
{
  m_gui.leName->setText(name);
}

QString AssetManagerRenameDialog::GetName() const
{
  return m_gui.leName->text();
}

void AssetManagerRenameDialog::on_pbOK_clicked()
{
  accept();
}

void AssetManagerRenameDialog::on_pbCancel_clicked()
{
  reject();
}