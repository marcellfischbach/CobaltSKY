
#pragma once

#include <qgraphicsitem.h>

namespace graph
{
class Node;
class NodeConnection : public QGraphicsPathItem
{
public:
  NodeConnection(Node *outputNode, int outputIdx, Node *inputNode, int inputIdx, QGraphicsItem *parent = 0);

  void Update();

  const Node *GetOutputNode() const
  {
    return m_outputNode;
  }

  const Node *GetInputNode() const
  {
    return m_inputNode;
  }

private:
  Node *m_outputNode;
  int m_outputIdx;

  Node *m_inputNode;
  int m_inputIdx;
};

}
