
#include <ShaderGraph/ShaderGraphScene.hh>
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <Graph/Node.hh>
#include <ShaderGraph/MetaData.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>


namespace shadergraph
{
ShaderGraphScene::ShaderGraphScene(QObject *parent)
  : graph::NodeGraphScene(parent)
  , m_shaderGraphNode(0)
{
  connect(this, SIGNAL(NodeAdded(graph::GraphNode *)), this, SLOT(NodeAdded(graph::GraphNode *)));
  connect(this, SIGNAL(NodeRemoved(graph::GraphNode *)), this, SLOT(NodeRemoved(graph::GraphNode *)));
  connect(this, SIGNAL(ConnectionAdded(graph::AnchorConnectionItem *)), this, SLOT(ConnectionAdded(graph::AnchorConnectionItem *)));
  connect(this, SIGNAL(ConnectionRemoved(graph::AnchorConnectionItem *)), this, SLOT(ConnectionRemoved(graph::AnchorConnectionItem *)));
}

void ShaderGraphScene::Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData)
{
  Clear();
  if (m_shaderGraphNode)
  {
    delete m_shaderGraphNode;
  }

  bool blocked = blockSignals(true);

  m_shaderGraphNode = new SGShaderGraphNode(shaderGraph);
  AddNode(m_shaderGraphNode);
  vkVector2f pos = metaData->GetGraphPosition();
  m_shaderGraphNode->setPos(QPointF (pos.x, pos.y));

  QMap<vkSGNode*, SGNode*> nodes;
  for (size_t i=0, in=shaderGraph->GetNumberOfTotalNodes(); i<in; ++i)
  {
    vkSGNode *node = shaderGraph->GetNode(i);
    if (node)
    {
      SGNode *sgNode = new SGNode(node);
      AddNode(sgNode);
      nodes[node] = sgNode;
      pos = metaData->GetNodePosition(i);
      sgNode->setPos(QPointF(pos.x, pos.y));
    }
  }

  for (size_t i=0, in=shaderGraph->GetNumberOfTotalNodes(); i<in; ++i)
  {
    vkSGNode *node = shaderGraph->GetNode(i);
    for (vkSize j=0, jn=node->GetNumberOfInputs(); j<jn; ++j)
    {
      vkSGInput* input = node->GetInput(j);
      vkSGNode *inputNode = input->GetNode();

      vkSGOutput *output = input->GetInput();
      if (output)
      {
        vkSGNode *outputNode = output->GetNode();
        if (nodes.contains(inputNode) && nodes.contains(outputNode))
        {
          SGNode *sgNodeInput = nodes[inputNode];
          SGNode *sgNodeOutput= nodes[outputNode];

          // can savely be casted as attrib input/output widget because those are the only input/outpus that are used within the shader graph
          graph::AttribInputWidget *inputWidget = static_cast<graph::AttribInputWidget*>(sgNodeInput->GetInput(input->GetIdx()));
          graph::AttribOutputWidget *outputWidget = static_cast<graph::AttribOutputWidget*>(sgNodeOutput->GetOutput(output->GetIdx()));

          AddConnection(outputWidget->GetAnchor(), inputWidget->GetAnchor());
        }
      }
    }
  }

  for (size_t i=0; i<vkSGShaderGraph::eIT_COUNT; ++i)
  {
    vkSGOutput* output = shaderGraph->GetInput((vkSGShaderGraph::InputType)i);
    if (output)
    {
      vkSGNode *outputNode = output->GetNode();
      if (nodes.contains(outputNode))
      {

        SGNode *sgNodeOutput= nodes[outputNode];

        // can savely be casted as attrib input/output widget because those are the only input/outpus that are used within the shader graph
        graph::AttribInputWidget *inputWidget = static_cast<graph::AttribInputWidget*>(m_shaderGraphNode->GetInput(i));
        graph::AttribOutputWidget *outputWidget = static_cast<graph::AttribOutputWidget*>(sgNodeOutput->GetOutput(output->GetIdx()));

        AddConnection(outputWidget->GetAnchor(), inputWidget->GetAnchor());
      }
    }

  }
  blockSignals(blocked);
}

SGShaderGraphNode *ShaderGraphScene::GetShaderGraphNode()
{
  return m_shaderGraphNode;
}

void ShaderGraphScene::OnConnectionAdded (graph::AnchorConnectionItem *connection)
{
  graph::NodeGraphScene::OnConnectionAdded(connection);
  graph::AnchorWidget *widget = connection->GetInputWidget();
  if (widget && widget->GetType() != graph::AnchorWidget::eT_Attrib)
  {
    widget = 0;
  }
  if (!widget)
  {
    widget = connection->GetOutputWidget();
    if (widget && widget->GetType() != graph::AnchorWidget::eT_Flow)
    {
      widget = 0;
    }
  }
  if (!widget)
  {
    return;
  }

  QList<graph::AnchorConnectionItem*> connections;
  for (size_t i=0, in=widget->GetNumberOfConnections(); i<in; ++i)
  {
    graph::AnchorConnectionItem *con = widget->GetConnection(i);
    if (con != connection)
    {
      connections.append(con);
    }
  }
  for (auto con : connections)
  {
    RemoveConnection(con);
  }

}

void ShaderGraphScene::OnConnectionCanceled(graph::AnchorWidget *anchor)
{
  if (anchor &&
      (
        anchor->GetType() == graph::AnchorWidget::eT_Attrib && anchor->GetDirection() == graph::AnchorWidget::eD_In ||
        anchor->GetType() == graph::AnchorWidget::eT_Flow && anchor->GetDirection() == graph::AnchorWidget::eD_Out
        )
      )
  {
    QList<graph::AnchorConnectionItem*> connections;
    for (size_t i=0, in=anchor->GetNumberOfConnections(); i<in; ++i)
    {
      graph::AnchorConnectionItem *con = anchor->GetConnection(i);
      connections.append(con);
    }
    for (auto con : connections)
    {
      RemoveConnection(con);
    }

  }
}

void ShaderGraphScene::NodeAdded(graph::GraphNode *node)
{
  Node *n = static_cast<Node*>(node);
  if (n->GetType() != shadergraph::Node::eT_Node)
  {
    return;
  }

  SGNode *sgNode = static_cast<SGNode*>(n);
  vkSGNode *vksgNode = sgNode->GetNode();
  m_shaderGraphNode->GetShaderGraph()->AddNode(vksgNode);

}

void ShaderGraphScene::NodeRemoved(graph::GraphNode *node)
{
  Node *n = static_cast<Node*>(node);
  if (n->GetType() != shadergraph::Node::eT_Node)
  {
    return;
  }

  SGNode *sgNode = static_cast<SGNode*>(n);
  vkSGNode *vksgNode = sgNode->GetNode();
  m_shaderGraphNode->GetShaderGraph()->RemoveNode(vksgNode);
}

void ShaderGraphScene::ConnectionAdded(graph::AnchorConnectionItem *anchor)
{
  if (!anchor || !anchor->IsValid())
  {
    return;
  }
  graph::AnchorWidget *outputWidget = anchor->GetOutputWidget();
  graph::AnchorWidget *inputWidget = anchor->GetInputWidget();
  int outputIdx = outputWidget->GetContentWidget()->GetIndex();
  int inputIdx = inputWidget->GetContentWidget()->GetIndex();

  Node *nO = static_cast<Node*>(outputWidget->GetGraphNode());
  Node *nI = static_cast<Node*>(inputWidget->GetGraphNode());

  if (nI->GetType() == shadergraph::Node::eT_ShaderGraph)
  {
    vkSGOutput *output = static_cast<SGNode*>(nO)->GetNode()->GetOutput(outputIdx);
    m_shaderGraphNode->GetShaderGraph()->SetInput((vkSGShaderGraph::InputType)inputIdx, output);
  }
  else
  {
    static_cast<SGNode*>(nI)->GetNode()->SetInput(inputIdx, static_cast<SGNode*>(nO)->GetNode(), outputIdx);
  }

}

void ShaderGraphScene::ConnectionRemoved(graph::AnchorConnectionItem *anchor)
{
  if (!anchor || !anchor->IsValid())
  {
    return;
  }
  graph::AnchorWidget *inputWidget = anchor->GetInputWidget();
  graph::AnchorWidget *outputWidget = anchor->GetOutputWidget();
  int inputIdx = inputWidget->GetContentWidget()->GetIndex();
  int outputIdx = outputWidget->GetContentWidget()->GetIndex();

  Node *nI = static_cast<Node*>(inputWidget->GetGraphNode());
  Node *nO = static_cast<Node*>(outputWidget->GetGraphNode());


  vkSGOutput *output = static_cast<SGNode*>(nO)->GetNode()->GetOutput(outputIdx);

  if (nI->GetType() == shadergraph::Node::eT_ShaderGraph)
  {
    vkSGOutput *currentOutput = m_shaderGraphNode->GetShaderGraph()->GetInput((vkSGShaderGraph::InputType)inputIdx);
    if (currentOutput == output)
    {
      m_shaderGraphNode->GetShaderGraph()->SetInput((vkSGShaderGraph::InputType)inputIdx, 0);
    }
  }
  else
  {
    vkSGNode *vksgNode = static_cast<SGNode*>(nI)->GetNode();
    vkSGInput *currentInput = vksgNode->GetInput(inputIdx);
    if (currentInput && currentInput->GetInput() == output)
    {
      static_cast<SGNode*>(nI)->GetNode()->SetInput(inputIdx, 0, 0);
    }
  }
}


}
