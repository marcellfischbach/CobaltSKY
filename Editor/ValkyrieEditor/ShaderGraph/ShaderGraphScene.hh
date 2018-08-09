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
  Q_OBJECT
public:
  ShaderGraphScene(QObject *parent = 0);

  void Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData);
  SGShaderGraphNode *GetShaderGraphNode();

  virtual void OnConnectionAdded (graph::AnchorConnectionItem *connection);
  virtual void OnConnectionCanceled(graph::AnchorWidget *anchor);

private slots:
  void NodeAdded(graph::GraphNode *node);
  void NodeRemoved(graph::GraphNode *node);

  void ConnectionAdded(graph::AnchorConnectionItem *anchor);
  void ConnectionRemoved(graph::AnchorConnectionItem *anchor);

private:


  SGShaderGraphNode* m_shaderGraphNode;

  QMap<vkSGNode*, SGNode*> m_nodes;


};

}

#endif // SHADERGRAPHSCENE_HH
