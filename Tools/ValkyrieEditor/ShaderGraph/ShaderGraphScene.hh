#ifndef SHADERGRAPHSCENE_HH
#define SHADERGRAPHSCENE_HH

#include <Graph/Scene.hh>
#include <QMap>


class vkSGNode;
class vkSGShaderGraph;
class ShaderGraphMetaData;

namespace shadergraph
{
class SGNode;
class SGShaderGraphNode;

class ShaderGraphScene : public graph::NodeGraphScene
{
public:
  ShaderGraphScene(QObject *parent = 0);

  void Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData);

  virtual void OnConnectionAdded (graph::AnchorConnectionItem *connection);
  virtual void OnConnectionCanceled(graph::AnchorWidget *anchor);

private:

  SGShaderGraphNode* m_shaderGraphNode;

  QMap<vkSGNode*, SGNode*> m_nodes;

};

}

#endif // SHADERGRAPHSCENE_HH
