
#include <assetmanager/menu/items/rename/renamedialog.hh>
#include <editormodel/nodes/node.hh>

namespace cs::editor::assetmanager
{

RenameDialog::RenameDialog(model::Node* node, const std::string &oldName, bool hasSuper, bool hasOverriders, QWidget *parent)
  : QDialog(parent)
{
  m_gui.setupUi(this);
  m_gui.message->setText(tr("Rename '%1'. Enter a new name").arg(node->GetName().c_str()));
  m_gui.leName->setText(QString(oldName.c_str()));
  m_gui.leName->selectAll();

  m_gui.cbRenameSuper->setEnabled(hasSuper);
  m_gui.cbRenameOverriders->setEnabled(hasOverriders);

  connect(m_gui.pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_gui.pbOK, SIGNAL(clicked()), this, SLOT(accept()));
  //connect (m_gui.p)

  setModal(true);
}

RenameDialog::~RenameDialog()
{

}

std::string RenameDialog::GetName() const
{
  return std::string(m_gui.leName->text().toLatin1().data());
}

bool RenameDialog::RenameSuper() const
{
  return m_gui.cbRenameSuper->isChecked();
}

bool RenameDialog::RenameOverriders() const
{
  return m_gui.cbRenameOverriders->isChecked();
}

}