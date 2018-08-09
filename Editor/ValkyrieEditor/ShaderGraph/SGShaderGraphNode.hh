#pragma once


#include <ShaderGraph/Node.hh>

class vkSGShaderGraph;

namespace graph
{
class Connection;
class AttribInputWidget;
}
namespace shadergraph
{

class SGShaderGraphNode : public Node
{
public:
  SGShaderGraphNode(vkSGShaderGraph *graph);



public:
  vkSGShaderGraph *GetShaderGraph()
  {
    return m_graph;
  }

  const vkSGShaderGraph *GetShaderGraph() const
  {
    return m_graph;
  }

private:
  vkSGShaderGraph *m_graph;

  graph::AttribInputWidget *m_inputDiffuse;
  graph::AttribInputWidget *m_inputAlpha;
  graph::AttribInputWidget *m_inputRoughness;
  graph::AttribInputWidget *m_inputNormal;
};

}
