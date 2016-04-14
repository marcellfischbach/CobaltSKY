
#pragma once

#include <qobject.h>
#include <qstring.h>
#include <qvector.h>
#include <Graph/Direction.h>


class QGraphicsItem;

namespace graph
{
class NodeInputItem;
class NodeOutputItem;
class NodeGroup;
class NodeLabel;
class NodeGraphScene;
class Node : public QObject
{
  friend class NodeBackground;
public:
  enum InputMode
  {
    eIM_Const = 0x01,
    eIM_Output = 0x02,
    eIM_Both = eIM_Const | eIM_Output
  };


  struct AnchorRequestResult
  {
    QPointF pos;
    int idx;
    QString key;
    Direction dir;
  };



public:
  Node(QObject *parent = 0);

  inline void SetLabel(const QString &label)
  {
    m_label = label;
  }

  void AddInput(const QString &label, const QString &key, InputMode mode);
  void AddOutput(const QString &label, const QString &key);

  int GetIndexOfInput(const QString &key) const;
  int GetIndexOfOutput(const QString &key) const;

  QPointF GetAnchorInputPos(int idx) const;
  QPointF GetAnchorOutputPos(int idx) const;

  bool Initialize();

  inline QGraphicsItem *GetItem()
  {
    return m_item;
  }

  bool TestAnchor(const QPointF &pos, AnchorRequestResult &result);

  void SetPosition(const QPointF &pos);

  void SetScene(NodeGraphScene *scene)
  {
    m_scene = scene;
  }
private:

  struct Input
  {
    QString label;
    QString key;
    InputMode mode;
    NodeInputItem *item;
  };

  struct Output
  {
    QString label;
    QString key;
    NodeOutputItem *item;
  };

  QString m_label;
  QVector<Input> m_inputs;
  QVector<Output> m_outputs;

  QGraphicsItem *m_item;
  NodeGroup *m_nodeGroup;
  NodeLabel *m_title;

  NodeGraphScene *m_scene;
};

}