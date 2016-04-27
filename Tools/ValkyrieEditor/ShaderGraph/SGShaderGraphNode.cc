

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
  AddInput("Alpha", "alpha", graph::Node::eIM_Output);
  AddInput("Roughness", "roughness", graph::Node::eIM_Output);
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
  }

  shadergraph::Node::AddConnection(connection);
}

void shadergraph::SGShaderGraphNode::RemoveConnection(graph::NodeConnection* connection)
{
  shadergraph::Node::RemoveConnection(connection);
}
