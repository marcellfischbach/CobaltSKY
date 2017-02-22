#pragma once

#include <nodegraph/nodegraphnode.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <map>

class vkSGNode;
class vkSGShaderGraph;
class NodeGraphNodeValueProperty;
class ShaderGraphEditorNode : public NodeGraphNode
{
public:
  ShaderGraphEditorNode(vkSGShaderGraph *shaderGraph);
  ShaderGraphEditorNode(vkSGNode *node);
  virtual ~ShaderGraphEditorNode();

  vkSGShaderGraph *GetShaderGraph() const;
  vkSGNode *GetSGNode() const;

  void UpdateConstInputValues();

private:
  vkSGShaderGraph *m_shaderGraph;
  vkSGNode *m_sgNode;

  std::map<unsigned, NodeGraphNodeValueProperty*> m_valueProperties;
};

