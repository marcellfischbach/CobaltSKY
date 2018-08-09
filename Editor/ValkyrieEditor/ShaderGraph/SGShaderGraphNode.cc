

#include <ShaderGraph/SGShaderGraphNode.hh>
#include <ShaderGraph/SGNode.hh>
#include <Graph/Node.hh>
#include <Graph/Scene.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>

shadergraph::SGShaderGraphNode::SGShaderGraphNode(vkSGShaderGraph *graph)
  : Node(eT_ShaderGraph)
  , m_graph(graph)
{
  SetLabel("Shader");
  SetHeadlineColor(QColor(128, 128, 128));

  m_inputDiffuse = new graph::AttribInputWidget();
  m_inputAlpha= new graph::AttribInputWidget();
  m_inputRoughness = new graph::AttribInputWidget();
  m_inputNormal = new graph::AttribInputWidget();

  m_inputDiffuse->SetName("Diffuse");
  m_inputDiffuse->SetIndex(vkSGShaderGraph::eIT_Diffuse);
  m_inputAlpha->SetName("Alpha");
  m_inputAlpha->SetIndex(vkSGShaderGraph::eIT_Alpha);
  m_inputRoughness->SetName("Roughness");
  m_inputRoughness->SetIndex(vkSGShaderGraph::eIT_Roughness);
  m_inputNormal->SetName("Normal");
  m_inputNormal->SetIndex(vkSGShaderGraph::eIT_Normal);

  AddInput(m_inputDiffuse);
  AddInput(m_inputAlpha);
  AddInput(m_inputRoughness);
  AddInput(m_inputNormal);


}

