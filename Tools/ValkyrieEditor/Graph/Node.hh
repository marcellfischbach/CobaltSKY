
#pragma once

#include <qcolor.h>
#include <qobject.h>
#include <qstring.h>
#include <qvector.h>
#include <Graph/Direction.h>


class QGraphicsItem;
namespace graph
{
class NodeConnection;
class NodeInputItem;
class NodeOutputItem;
class NodeGroup;
class NodeLabel;
class NodeGraphScene;
class NodeNameInput;
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

  inline const QString &GetLabel() const
  {
    return m_label;
  }

  inline void SetBackgroundColor(const QColor &color)
  {
    m_color = color;
  }

  inline const QColor &GetBackgroundColor() const
  {
    return m_color;
  }

  inline void SetHasName(bool hasName)
  {
    m_hasName = hasName;
  }

  inline bool HasName() const
  {
    return m_hasName;
  }

  inline void SetMinWidth(float minWidth)
  {
    m_minWidth = minWidth;
  }

  inline float GetMinWidth() const
  {
    return m_minWidth;
  }

  inline void SetName(const QString &name)
  {
    m_name = name;
  }
  QString GetName() const;

  void AddInput(const QString &label, const QString &key, InputMode mode);
  void AddOutput(const QString &label, const QString &key);

  int GetIndexOfInput(const QString &key) const;
  int GetIndexOfOutput(const QString &key) const;

  QString GetInputKey(int idx) const;
  QString GetOutputKey(int idx) const;

  QPointF GetAnchorInputPos(int idx) const;
  QPointF GetAnchorOutputPos(int idx) const;

  size_t GetNumberOfInputs() const;
  float GetConstInput(size_t idx) const;

  bool Initialize();

  inline QGraphicsItem *GetItem()
  {
    return m_item;
  }

  bool TestAnchor(const QPointF &pos, AnchorRequestResult &result);

  void SetPosition(const QPointF &pos);

  inline void SetScene(NodeGraphScene *scene)
  {
    m_scene = scene;
  }

  inline NodeGraphScene *GetScene()
  {
    return m_scene;
  }

  void UpdateSelection();

  void ResetConstVisible();
  void SetConstVisible(int input, bool visible);

  virtual void AddConnection(NodeConnection *connection);
  virtual void RemoveConnection(NodeConnection *connection);

  virtual void RemoveAllConnections();

  static void Select(Node *node);
  static Node *GetSelected();
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
  QColor m_color;

  bool m_hasName;
  QString m_name;
  NodeNameInput *m_nodeNameInput;

  float m_minWidth;

  QVector<Input> m_inputs;
  QVector<Output> m_outputs;

  QGraphicsItem *m_item;
  NodeGroup *m_nodeGroup;
  NodeLabel *m_title;

  NodeGraphScene *m_scene;
  bool m_valid;

  static Node *selectedNode;
protected:
  QVector<NodeConnection*> m_connections;

};

}