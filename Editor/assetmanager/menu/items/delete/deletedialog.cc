
#include <assetmanager/menu/items/delete/deletedialog.hh>
#include <editormodel/nodes/node.hh>

namespace cs::editor::assetmanager
{

DeleteDialog::DeleteDialog(model::Node* node, bool hasSuper, bool hasOverriders, QWidget *parent)
  : QDialog(parent)
{
  m_gui.setupUi(this);
  m_gui.message->setText(tr("Delete '%1'").arg(node->GetName().c_str()));

  m_gui.cbDeleteSuper->setEnabled(hasSuper);
  m_gui.cbDeleteOverriders->setEnabled(hasOverriders);
  m_gui.pbOK->setFocus();

  connect(m_gui.pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_gui.pbOK, SIGNAL(clicked()), this, SLOT(accept()));
  //connect (m_gui.p)

 
  setModal(true);
}

DeleteDialog::~DeleteDialog()
{

}


bool DeleteDialog::DeleteSuper() const
{
  return m_gui.cbDeleteSuper->isChecked();
}

bool DeleteDialog::DeleteOverriders() const
{
  return m_gui.cbDeleteOverriders->isChecked();
}

}