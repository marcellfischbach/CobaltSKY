
#pragma once

#include <qcolor.h>
#include <qobject.h>
#include <qstring.h>
#include <qvector.h>
#include <Graph/Direction.h>
#include <Valkyrie/Types.hh>


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
  Q_OBJECT
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

  void SetLabel(const QString &label);
  const QString &GetLabel() const;

  void SetBackgroundColor(const QColor &color);
  const QColor &GetBackgroundColor() const;

  void SetHasName(bool hasName);
  bool HasName() const;

  void SetMinWidth(float minWidth);
  float GetMinWidth() const;

  void SetName(const QString &name);
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
  void SetConstInput(size_t idx, float constValue);

  virtual bool Initialize();

  QGraphicsItem *GetItem();

  bool TestAnchor(const QPointF &pos, AnchorRequestResult &result);

  void SetPosition(const QPointF &pos);

  void SetScene(NodeGraphScene *scene);
  NodeGraphScene *GetScene();


  void UpdateSelection();

  void ResetConstVisible();
  void SetConstVisible(int input, bool visible);

  QVector<NodeConnection*> GetAllConnections() const;
  virtual void AddConnection(NodeConnection *connection);
  virtual void RemoveConnection(NodeConnection *connection);

  virtual void RemoveAllConnections();

  static void Select(Node *node);
  static Node *GetSelected();

  void SetIdx(vkUInt32 idx);
  vkUInt32 GetIdx() const;


private slots:
void NameChanged();

private:
  vkUInt32 m_idx;

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


inline void Node::SetLabel(const QString &label)
{
  m_label = label;
}

inline const QString &Node::GetLabel() const
{
  return m_label;
}

inline void Node::SetBackgroundColor(const QColor &color)
{
  m_color = color;
}

inline const QColor &Node::GetBackgroundColor() const
{
  return m_color;
}

inline void Node::SetHasName(bool hasName)
{
  m_hasName = hasName;
}

inline bool Node::HasName() const
{
  return m_hasName;
}

inline void Node::SetMinWidth(float minWidth)
{
  m_minWidth = minWidth;
}

inline float Node::GetMinWidth() const
{
  return m_minWidth;
}

inline void Node::SetName(const QString &name)
{
  m_name = name;
}

inline void Node::SetScene(NodeGraphScene *scene)
{
  m_scene = scene;
}

inline NodeGraphScene *Node::GetScene()
{
  return m_scene;
}

inline QGraphicsItem *Node::GetItem()
{
  return m_item;
}

inline void Node::SetIdx(vkUInt32 idx)
{
  m_idx = idx;
}

inline vkUInt32 Node::GetIdx() const
{
  return m_idx;
}

}