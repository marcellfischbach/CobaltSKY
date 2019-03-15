
#include <assetmanager/menu/items/newfolder/newfolderdialog.hh>


namespace cs::editor::assetmanager
{

NewFolderDialog::NewFolderDialog(QWidget *parent)
  : QDialog(parent)
{
  m_gui.setupUi(this);
  setModal(true);

  connect(m_gui.pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_gui.pbOK, SIGNAL(clicked()), this, SLOT(accept()));

}

NewFolderDialog::~NewFolderDialog()
{

}

std::string NewFolderDialog::GetFolderName() const
{
  return std::string(m_gui.leFolderName->text().toLatin1().data());
}

}