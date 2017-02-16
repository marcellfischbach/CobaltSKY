#pragma once

#include <nodegraph/nodegraphnode.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>

class vkSGNode;
class vkSGShaderGraph;
class ShaderGraphEditorNode : public NodeGraphNode
{
public:
  ShaderGraphEditorNode(vkSGShaderGraph *shaderGraph);
  ShaderGraphEditorNode(vkSGNode *node);
  virtual ~ShaderGraphEditorNode();

  vkSGShaderGraph *GetShaderGraph() const;
  vkSGNode *GetSGNode() const;

private:
  vkSGShaderGraph *m_shaderGraph;
  vkSGNode *m_sgNode;
};

