#include <AssetManager/NewAssetDialog.hh>
#include <Editor.hh>

namespace assetmanager
{
NewAssetDialog::NewAssetDialog(QWidget *parent)
  : QDialog(parent)
{
  setModal(true);
  m_gui.setupUi(this);
  setWindowTitle(Editor::Get().GetTitleName());
}


QString NewAssetDialog::GetName() const
{
  return m_gui.lineEdit->text();
}

void NewAssetDialog::on_btnOK_clicked(bool)
{
  accept();
}

void NewAssetDialog::on_btnCancel_clicked(bool)
{
  reject();
}

}

