
#include <editors/shadergrapheditor/shadergrapheditortoolbox.hh>
#include <editors/shadergrapheditor/shadergrapheditortoolboxmodel.hh>

ShaderGraphEditorToolbox::ShaderGraphEditorToolbox()
  : QWidget()
{
  m_gui.setupUi(this);
  m_model = new ShaderGraphEditorToolboxModel();
  m_gui.treeView->setModel(m_model);
}

ShaderGraphEditorToolbox::~ShaderGraphEditorToolbox()
{

}


void ShaderGraphEditorToolbox::on_leFilter_textChanged(const QString &filter)
{
  m_model->CreateModelData(filter);
  if (!filter.isEmpty())
  {
    m_gui.treeView->expandAll();
  }
}
