
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>
#include <shadergrapheditor/shadergrapheditortoolboxmodel.hh>

#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>

#include <valkyrie/vkengine.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>

#include <QDropEvent>

ShaderGraphEditorWidget::ShaderGraphEditorWidget(ShaderGraphEditor *editor)
  : QWidget()
  , m_editor(editor)
  , m_shaderGraph(0)
  , m_shaderGraphCopy(new vkSGShaderGraph())
  , m_updateGuard(false)
{
  m_gui.setupUi(this);
  on_nodeGraph_ScaleChanged(1.0f);

  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(shaderGraph);
  m_gui.nodeGraph->AddNode(shaderGraphNode);
}

ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  VK_RELEASE(m_shaderGraph);
  VK_RELEASE(m_shaderGraphCopy);
}

void ShaderGraphEditorWidget::SetShaderGraph(vkSGShaderGraph *shaderGraph, ShaderGraphEditorMeta *meta)
{
  VK_SET(m_shaderGraph, shaderGraph);
  if (!m_shaderGraph)
  {
    return;
  }

  m_updateGuard = true;
  m_shaderGraphCopy = m_shaderGraph->Copy(m_shaderGraphCopy);

  m_gui.nodeGraph->Clear();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(m_shaderGraphCopy);
  vkVector2f shaderGraphNodePos = meta->GetPos();
  shaderGraphNode->SetLocation(QPointF(shaderGraphNodePos.x, shaderGraphNodePos.y));
  m_gui.nodeGraph->AddNode(shaderGraphNode);

  std::map<vkSGNode *, ShaderGraphEditorNode*> mapping;
  for (vkSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *node = m_shaderGraphCopy->GetNode(i);

    ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(node);
    vkVector2f nodePos = meta->GetPos(i);
    editorNode->SetLocation(QPointF(nodePos.x, nodePos.y));
    m_gui.nodeGraph->AddNode(editorNode);

    mapping[node] = editorNode;
  }

  for (vkSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *node = m_shaderGraphCopy->GetNode(i);
    for (vkSize j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput* input = node->GetInput(j);
      vkSGOutput *output = input->GetInput();
      if (output)
      {
        vkSGNode *outputNode = output->GetNode();

        ShaderGraphEditorNode* editorNode = mapping[node];
        ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

        NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
        NodeGraphNodeAnchor *inputAnchor = editorNode->GetInputAnchor(input->GetIdx());

        m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
      }
    }
  }

  for (vkSize i = 0; i < vkSGShaderGraph::eIT_COUNT; ++i) 
  {
    vkSGShaderGraph::InputType inputType = (vkSGShaderGraph::InputType)i;
    vkSGOutput *output = m_shaderGraphCopy->GetInput(inputType);
    if (output)
    {
      vkSGNode *outputNode = output->GetNode();
      ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

      NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
      NodeGraphNodeAnchor *inputAnchor = shaderGraphNode->GetInputAnchor(i);

      m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
    }

  }
  m_updateGuard = false;
}


void ShaderGraphEditorWidget::RepaintGraph()
{
  m_gui.nodeGraph->repaint();
}


void ShaderGraphEditorWidget::on_nodeGraph_CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto)
{
  if (!anchorA 
      || !anchorB
      || anchorA->GetType() == anchorB->GetType()
      || anchorA->GetNode() == anchorB->GetNode())
  {
    veto->Veto();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (anchorA->GetType() == eNGNPT_Input)
  { 
    m_gui.nodeGraph->DisconnectAll(anchorA);
  }
  if (anchorB->GetType() == eNGNPT_Input)
  {
    m_gui.nodeGraph->DisconnectAll(anchorB);
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_Connected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (m_updateGuard)
  {
    return;
  }
  NodeGraphNodeAnchor *outputAnchor = anchorA->GetType() == eNGNPT_Output ? anchorA : anchorB;
  NodeGraphNodeAnchor *inputAnchor = anchorA->GetType() == eNGNPT_Input ? anchorA : anchorB;

  ShaderGraphEditorNode *editorNodeOutput = static_cast<ShaderGraphEditorNode*>(outputAnchor->GetNode());
  ShaderGraphEditorNode *editorNodeInput = static_cast<ShaderGraphEditorNode*>(inputAnchor->GetNode());

  if (editorNodeInput->GetShaderGraph())
  {
    // make the connection to the shader graph
    vkSGNode *nodeOutput = editorNodeOutput->GetSGNode();
    vkSGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    vkSGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((vkSGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), output);
  }
  else
  {
    // make the connection between two nodes
    vkSGNode *nodeOutput = editorNodeOutput->GetSGNode();
    vkSGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    vkSGNode *nodeInput = editorNodeInput->GetSGNode();
    vkSGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());

    input->SetInput(output);
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_Disconnected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (m_updateGuard)
  {
    return;
  }

  printf("Disconnected: %p %p\n", anchorA, anchorB);
  NodeGraphNodeAnchor *inputAnchor = anchorA->GetType() == eNGNPT_Input ? anchorA : anchorB;

  ShaderGraphEditorNode *editorNodeInput = static_cast<ShaderGraphEditorNode*>(inputAnchor->GetNode());

  if (editorNodeInput->GetShaderGraph())
  {
    // make the connection to the shader graph
    vkSGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((vkSGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), 0);
  }
  else
  {
    // make the connection between two nodes
    vkSGNode *nodeInput = editorNodeInput->GetSGNode();
    vkSGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());
    input->SetInput(0);
  }
}



void ShaderGraphEditorWidget::on_nodeGraph_CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto)
{
  if (anchor->GetType() == eNGNPT_Output)
  {
    veto->Veto();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_ScaleChanged(float scale)
{
  int iscale = scale * 100.0f;
  m_gui.lblScale->setText(QString("%1%").arg(iscale));

}

void ShaderGraphEditorWidget::on_nodeGraph_CheckDrag(const QDropEvent *event, NodeGraphAcceptEvent *accept)
{
  if (event->mimeData()->hasFormat(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME))
  {
    accept->Accept();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_DragDropped(const QDropEvent *event)
{
  if (event->mimeData()->hasFormat(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME))
  {
    QByteArray ba = event->mimeData()->data(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME);
    QDataStream stream(&ba, QIODevice::ReadOnly);
    QString className;
    stream >> className;
    const vkClass *cls = vkClassRegistry::Get()->GetClass(vkString((const char*)className.toLatin1()));
    if (!cls)
    {
      return;
    }

    vkSGNode *node = cls->CreateInstance<vkSGNode>();
    if (!node)
    {
      return;
    }

    m_shaderGraphCopy->AddNode(node);

    ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(node);
    m_gui.nodeGraph->AddNode(editorNode);
    editorNode->SetLocation(m_gui.nodeGraph->GetLocalCoordinate(event->pos()));
    m_gui.nodeGraph->repaint();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_RequestRemoveNode(QList<NodeGraphNode*> nodes, NodeGraphVetoEvent *veto)
{
  for (NodeGraphNode* node : nodes)
  {
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(node);
    if (editorNode->GetShaderGraph())
    {
      veto->Veto();
      return;
    }
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_NodeRemoved(NodeGraphNode* node)
{
  if (!node)
  {
    return;
  }
  printf("RemoveNode: %p\n", node);
  ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(node);
  vkSGNode *sgNode = editorNode->GetSGNode();
  if (!sgNode)
  {
    m_shaderGraphCopy->RemoveNode(sgNode);
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_SelectionChanged(const QList<NodeGraphNode*> &nodes)
{
  QList<ShaderGraphEditorNode*> editorNodes;
  for (NodeGraphNode *node : nodes)
  {
    editorNodes.push_back(static_cast<ShaderGraphEditorNode*>(node));
  }

  emit SelectionChanged(editorNodes);
}

void ShaderGraphEditorWidget::on_pbApply_clicked()
{
  if (vkEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraphCopy))
  {
    printf("Successfully compiled\n");
  }
  else
  {
    printf("Compiled with errors\n");
    return;
  }

  m_shaderGraph = m_shaderGraphCopy->Copy(m_shaderGraph);
  vkEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraph);
}