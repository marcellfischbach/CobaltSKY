
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

  Node *GetOutputNode() 
  {
    return m_outputNode;
  }

  int GetOutputIdx() const
  {
    return m_outputIdx;
  }

  const Node *GetInputNode() const
  {
    return m_inputNode;
  }

  Node *GetInputNode() 
  {
    return m_inputNode;
  }

  int GetInputIdx() const
  {
    return m_inputIdx;
  }
private:
  Node *m_outputNode;
  int m_outputIdx;

  Node *m_inputNode;
  int m_inputIdx;
};

}
