
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

}

void ShaderGraphScene::Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData)
{
  Clear();
  if (m_shaderGraphNode)
  {
    delete m_shaderGraphNode;
  }

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

}

void ShaderGraphScene::OnConnectionAdded (graph::AnchorConnectionItem *connection)
{
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

}
