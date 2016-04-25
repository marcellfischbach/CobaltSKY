

#include <ShaderGraph/SGShaderGraphNode.hh>
#include <ShaderGraph/SGNode.hh>
#include <Graph/Connection.hh>
#include <Graph/Scene.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>

shadergraph::SGShaderGraphNode::SGShaderGraphNode(vkSGShaderGraph *graph)
  : Node(eT_ShaderGraph)
  , m_graph(graph)
{
  SetLabel("Shader");
  SetBackgroundColor(QColor(128, 128, 128));
  AddInput("Diffuse", "diffuse", graph::Node::eIM_Output);
  AddInput("Alpha", "alpha", graph::Node::eIM_Both);
  AddInput("Roughness", "roughness", graph::Node::eIM_Both);
  AddInput("Normal", "normal", graph::Node::eIM_Output);

}
void shadergraph::SGShaderGraphNode::AddConnection(graph::NodeConnection* connection)
{
  if (connection->GetInputNode() == this)
  {
    if (GetScene())
    {
      GetScene()->DisconnectInput(this, connection->GetInputIdx());
    }

    SGNode *sgOutput = static_cast<SGNode*>(connection->GetOutputNode());
    if (sgOutput->GetType() == SGNode::eT_Node)
    {
      vkSGNode *outputNode = static_cast<SGNode*>(sgOutput)->GetNode();
      vkSGOutput *output = outputNode->GetOutput(connection->GetOutputIdx());
      m_graph->SetInput((vkSGShaderGraph::InputType)connection->GetInputIdx(), output);
    }
  }

  shadergraph::Node::AddConnection(connection);
}

void shadergraph::SGShaderGraphNode::RemoveConnection(graph::NodeConnection* connection)
{

  if (connection->GetInputNode() == this)
  {
    m_graph->SetInput((vkSGShaderGraph::InputType)connection->GetInputIdx(), 0);
  }


  shadergraph::Node::RemoveConnection(connection);
}
