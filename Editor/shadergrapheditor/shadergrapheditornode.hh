#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/components/nodegraph/nodegraphnode.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <map>

namespace
{
class SGNode;
class SGShaderGraph;
}
class NodeGraphNodeValueProperty;
class ShaderGraphEditorNode : public NodeGraphNode
{
public:
  ShaderGraphEditorNode(cs::SGShaderGraph *shaderGraph);
  ShaderGraphEditorNode(cs::SGNode *node);
  virtual ~ShaderGraphEditorNode();

  NodeGraphNodeAnchor* GetInputAnchor(csUInt32 idx) const;
  NodeGraphNodeAnchor* GetOutputAnchor(csUInt32 idx) const;

  cs::SGShaderGraph *GetShaderGraph() const;
  cs::SGNode *GetSGNode() const;

  void UpdateValues();

private:
  cs::SGShaderGraph *m_shaderGraph;
  cs::SGNode *m_sgNode;

  cs::ResourceLocator m_texturePreviewResourceLocator;

  std::map<unsigned, NodeGraphNodeValueProperty*> m_valueProperties;
  std::map<csUInt32, NodeGraphNodeAnchor *> m_inputAnchors;
  std::map<csUInt32, NodeGraphNodeAnchor *> m_outputAnchors;
};

