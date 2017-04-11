#pragma once

#include <nodegraph/nodegraphnode.hh>
#include <valkyrie/core/vkresourcelocator.hh>
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

  NodeGraphNodeAnchor* GetInputAnchor(vkUInt32 idx) const;
  NodeGraphNodeAnchor* GetOutputAnchor(vkUInt32 idx) const;

  vkSGShaderGraph *GetShaderGraph() const;
  vkSGNode *GetSGNode() const;

  void UpdateValues();

private:
  vkSGShaderGraph *m_shaderGraph;
  vkSGNode *m_sgNode;

  vkResourceLocator m_texturePreviewResourceLocator;

  std::map<unsigned, NodeGraphNodeValueProperty*> m_valueProperties;
  std::map<vkUInt32, NodeGraphNodeAnchor *> m_inputAnchors;
  std::map<vkUInt32, NodeGraphNodeAnchor *> m_outputAnchors;
};

