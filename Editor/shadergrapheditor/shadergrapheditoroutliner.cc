
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
    cs::SGShaderGraph *shaderGraph = editorNode->GetShaderGraph();
    if (shaderGraph)
    {
      for (unsigned i = 0; i < columnCount; ++i)
      {
        m_gui.treeView->selectionModel()->select(m_model->GetIndex(shaderGraph, i), QItemSelectionModel::Select);
      }
    }

    cs::SGNode *node = editorNode->GetSGNode();
    if (node)
    {
      for (unsigned i = 0; i < columnCount; ++i)
      {
        m_gui.treeView->selectionModel()->select(m_model->GetIndex(node, i), QItemSelectionModel::Select);
      }
    }
  }
}

void ShaderGraphEditorOutliner::NodeChanged(ShaderGraphEditorNode* node)
{
  if (node)
  {
    m_model->NodeChanged(node->GetSGNode());
  }
}

void ShaderGraphEditorOutliner::NodeAboutToAdd(cs::SGNode *node)
{
  m_model->NodeAboutToAdd(node);
}


void ShaderGraphEditorOutliner::NodeAdded(cs::SGNode *node)
{
  m_model->NodeAdded(node);
}

void ShaderGraphEditorOutliner::NodeAboutToRemove(cs::SGNode *node)
{
  m_model->NodeAboutToRemove(node);
}

void ShaderGraphEditorOutliner::NodeRemoved(cs::SGNode *node)
{
  m_model->NodeRemoved(node);
}


void ShaderGraphEditorOutliner::SetShaderGraph(cs::SGShaderGraph *shaderGraph)
{
  m_model->SetShaderGraph(shaderGraph);
}
