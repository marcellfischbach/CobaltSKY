
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
    csSGShaderGraph *shaderGraph = editorNode->GetShaderGraph();
    if (shaderGraph)
    {
      for (unsigned i = 0; i < columnCount; ++i)
      {
        m_gui.treeView->selectionModel()->select(m_model->GetIndex(shaderGraph, i), QItemSelectionModel::Select);
      }
    }

    csSGNode *node = editorNode->GetSGNode();
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

void ShaderGraphEditorOutliner::NodeAboutToAdd(csSGNode *node)
{
  m_model->NodeAboutToAdd(node);
}


void ShaderGraphEditorOutliner::NodeAdded(csSGNode *node)
{
  m_model->NodeAdded(node);
}

void ShaderGraphEditorOutliner::NodeAboutToRemove(csSGNode *node)
{
  m_model->NodeAboutToRemove(node);
}

void ShaderGraphEditorOutliner::NodeRemoved(csSGNode *node)
{
  m_model->NodeRemoved(node);
}


void ShaderGraphEditorOutliner::SetShaderGraph(csSGShaderGraph *shaderGraph)
{
  m_model->SetShaderGraph(shaderGraph);
}
