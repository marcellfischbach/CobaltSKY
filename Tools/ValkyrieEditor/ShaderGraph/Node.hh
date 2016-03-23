
#pragma once

#include <qgraphicsitem.h>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>


class ShaderGraphNodeAnchor;
class ShaderGraphNodeGroup;
class ShaderGraphConnection;
class ShaderGraphNode : public QObject
{
  friend class ShaderGraphNodeAnchor;
  Q_OBJECT
public:
  ShaderGraphNode (vkSGNode *node);

  QGraphicsItem *GetItem ();

  bool hasAnchor (bool input, const QPointF &globalP, QPointF &globalOut, int &index) const;

  QPointF GetInputAnchor(int idx) const;
  QPointF GetOutputAnchor(int idx) const;

  void UpdateConnections ();
  void ConnectInput (ShaderGraphConnection *connection, int idx);
  void ConnectOutput (ShaderGraphConnection *connection, int idx);

  void SetSelected (bool selected);
  static void Select (ShaderGraphNode *node);
private:
  void onMoveDrag(bool input, int i, const QPointF &pointA, const QPointF &pointB);
  void onStopDrag (bool input, int i, const QPointF &pointA, const QPointF &pointB);

private:
  ShaderGraphNodeGroup *m_parent;
  float m_titleFontSize;
  float m_inoutFontSize;
  QGraphicsRectItem *m_background;
  QGraphicsSimpleTextItem *m_title;

  struct Anchor
  {
    ShaderGraphNodeAnchor *anchor;
    QGraphicsSimpleTextItem *text;
    QGraphicsTextItem *editValue;
    QGraphicsRectItem *editValueBackground;
    ShaderGraphConnection *connection;
  };

  QList<Anchor> m_inputs;
  QList<Anchor> m_outputs;

  vkSGNode *m_node;

  bool m_selected;

  static ShaderGraphNode *currentSelectedNode;


signals:
  void moveDrag(bool input, int i, const QPointF &pointA, const QPointF &pointB);
  void stopDrag (bool input, int i, const QPointF &pointA, const QPointF &pointB);
};
