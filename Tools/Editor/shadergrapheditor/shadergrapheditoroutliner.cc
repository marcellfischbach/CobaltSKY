
#include <shadergrapheditor/shadergrapheditoroutliner.hh>
#include <shadergrapheditor/shadergrapheditoroutlinertablemodel.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>

ShaderGraphEditorOutliner::ShaderGraphEditorOutliner(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
  m_model = new ShaderGraphEditorOutlinerTableModel();
  m_gui.treeView->setModel(m_model);
}

ShaderGraphEditorOutliner::~ShaderGraphEditorOutliner()
{

}

void ShaderGraphEditorOutliner::SetSelectedNodes(const QList<ShaderGraphEditorNode*>& nodes)
{
  unsigned columnCount = m_model->columnCount();
  m_gui.treeView->clearSelection();
  for (ShaderGraphEditorNode *editorNode : nodes)
  {
    vkSGShaderGraph *shaderGraph = editorNode->GetShaderGraph();
    if (shaderGraph)
    {
      for (unsigned i = 0; i < columnCount; ++i)
      {
        m_gui.treeView->selectionModel()->select(m_model->GetIndex(shaderGraph, i), QItemSelectionModel::Select);
      }
    }

    vkSGNode *node = editorNode->GetSGNode();
    if (node)
    {
      for (unsigned i = 0; i < columnCount; ++i)
      {
        m_gui.treeView->selectionModel()->select(m_model->GetIndex(node, i), QItemSelectionModel::Select);
      }
    }
  }
}


void ShaderGraphEditorOutliner::SetShaderGraph(vkSGShaderGraph *shaderGraph)
{
  m_model->SetShaderGraph(shaderGraph);
}
