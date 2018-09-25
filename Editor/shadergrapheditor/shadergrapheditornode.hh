#pragma once

#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/components/nodegraph/nodegraphnode.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <map>

class csSGNode;
class csSGShaderGraph;
class NodeGraphNodeValueProperty;
class ShaderGraphEditorNode : public NodeGraphNode
{
public:
  ShaderGraphEditorNode(csSGShaderGraph *shaderGraph);
  ShaderGraphEditorNode(csSGNode *node);
  virtual ~ShaderGraphEditorNode();

  NodeGraphNodeAnchor* GetInputAnchor(csUInt32 idx) const;
  NodeGraphNodeAnchor* GetOutputAnchor(csUInt32 idx) const;

  csSGShaderGraph *GetShaderGraph() const;
  csSGNode *GetSGNode() const;

  void UpdateValues();

private:
  csSGShaderGraph *m_shaderGraph;
  csSGNode *m_sgNode;

  csResourceLocator m_texturePreviewResourceLocator;

  std::map<unsigned, NodeGraphNodeValueProperty*> m_valueProperties;
  std::map<csUInt32, NodeGraphNodeAnchor *> m_inputAnchors;
  std::map<csUInt32, NodeGraphNodeAnchor *> m_outputAnchors;
};

