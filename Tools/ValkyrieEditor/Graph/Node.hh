
#pragma once

#include <qcolor.h>
#include <qobject.h>
#include <qstring.h>
#include <qvector.h>
#include <Valkyrie/Types.hh>
#include <QImage>
#include <QGraphicsGridLayout>
#include <QGraphicsPixmapItem>
#include <QGraphicsWidget>
#include <QGraphicsPathItem>


class QGraphicsItem;
namespace graph
{
class ContentWidget;
class AnchorWidget;
class GraphNode;
class TextItem : public QGraphicsLayoutItem, public QGraphicsSimpleTextItem
{
public:
  TextItem (QGraphicsItem *parent);
  ~TextItem();

  void SetColor(const QColor &color);

  void setGeometry(const QRectF &geom) Q_DECL_OVERRIDE;
  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE;
  QRectF boundingRect() const Q_DECL_OVERRIDE;

  void SetGraphNode (GraphNode *graphNode);

private:
  GraphNode *m_graphNode;
};

class SpacerItem : public QGraphicsLayoutItem
{
public:
  SpacerItem (QGraphicsLayoutItem *parent, int direction);

  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE;

  void SetGraphNode (GraphNode *graphNode);

private:
  GraphNode *m_graphNode;
};

class AnchorConnectionItem : public QObject, public QGraphicsPathItem
{
  Q_OBJECT
public:
  AnchorConnectionItem(QGraphicsItem *parent, AnchorWidget *anchorA, AnchorWidget *anchorB);
  ~AnchorConnectionItem ();

  AnchorWidget *GetInputWidget ();
  AnchorWidget *GetOutputWidget ();

  AnchorWidget *GetAnchorA();
  AnchorWidget *GetAnchorB();

  bool IsEqual(AnchorWidget *anchorA, AnchorWidget *anchorB);

  bool IsValid();

private slots:
  void AnchorChanged();

private:
  AnchorWidget *m_anchorA;
  AnchorWidget *m_anchorB;

};

class AnchorWidget : public QGraphicsWidget
{
  Q_OBJECT
public:
  enum Type
  {
    eT_Attrib,
    eT_Flow,
  };

  enum Direction
  {
    eD_In,
    eD_Out,
  };

public:
  AnchorWidget (Type type, Direction direction, QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~AnchorWidget();

  void SetVisible (bool visible);
  bool IsVisible () const;
  void SetGraphNode (GraphNode *graphNode);
  void SetContentWidget(ContentWidget *contentWidget);
  void Hover(const QPointF &scenePos);
  void AddConnection (AnchorConnectionItem *connection);
  void RemoveConnection (AnchorConnectionItem *connection);
  AnchorWidget *FindAnchorWidget(const QPointF &scenePos);

  Type GetType () const;
  Direction GetDirection () const;
  GraphNode *GetGraphNode ();
  ContentWidget *GetContentWidget();

  size_t GetNumberOfConnections () const;
  AnchorConnectionItem *GetConnection (size_t idx);

  bool IsConnected () const;

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


protected:
  QList<AnchorConnectionItem*> m_connections;
  bool m_connected;
  bool m_hover;
  bool m_visible;

  Type m_type;
  Direction m_direction;
  ContentWidget *m_contentWidget;
  GraphNode *m_graphNode;

signals:
  void ConnectionChanged ();
};



class FlowAnchorWidget : public AnchorWidget
{
public:
  FlowAnchorWidget (Direction direction, QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~FlowAnchorWidget();

  void SetColor(const QColor &color);


  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
  QColor m_color;
};

class AttribAnchorWidget : public AnchorWidget
{
public:

public:
  AttribAnchorWidget (Direction direction, QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~AttribAnchorWidget();

  void SetColor(const QColor &color);

  QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
  QColor m_color;
};



class ContentWidget : public QGraphicsWidget
{
public:
  enum Type
  {
    eT_Attribute,
    eT_Flow,
  };

  enum Mode
  {
    eM_Input,
    eM_Output,
  };

public:
  ContentWidget(Type type, Mode mode, QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~ContentWidget();

  virtual void Hover(const QPointF &scenePos) = 0;
  virtual void SetGraphNode (GraphNode *graphNode);
  virtual AnchorWidget *FindAnchorWidget(const QPointF &scenePos) = 0;

  void SetIndex (int index);
  int GetIndex () const;

  Type GetType () const;
  Mode GetMode() const;

protected:
  GraphNode *m_graphNode;

  Type m_type;
  Mode m_mode;
  int m_index;
};


class AttribInputWidget : public ContentWidget
{
  Q_OBJECT
public:
  AttribInputWidget(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~AttribInputWidget();

  void SetName(const QString &name);
  void SetValue (const QString &value);
  void SetColor(const QColor &color);
  virtual void Hover(const QPointF &scenePos);
  virtual void SetGraphNode (GraphNode *graphNode);
  virtual AnchorWidget *FindAnchorWidget (const QPointF &scenePos);

  AttribAnchorWidget *GetAnchor ();

private:
  void UpdateText ();

private slots:
  void AnchorConnectionChanged ();

private:
  AttribAnchorWidget *m_anchor;
  TextItem *m_text;

  QString m_value;
  QString m_name;


};


class AttribOutputWidget : public ContentWidget
{
public:
  AttribOutputWidget(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~AttribOutputWidget();

  void SetName(const QString &name);
  void SetColor(const QColor &color);
  virtual void Hover(const QPointF &scenePos);
  virtual void SetGraphNode (GraphNode *graphNode);
  virtual AnchorWidget *FindAnchorWidget (const QPointF &scenePos);

  AttribAnchorWidget *GetAnchor ();

private:
  TextItem *m_text;
  AttribAnchorWidget *m_anchor;

  QString m_name;
};



class FlowOutputWidget : public ContentWidget
{
public:
  FlowOutputWidget(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  ~FlowOutputWidget();

  void SetName(const QString &name);
  void Hover(const QPointF &scenePos);
  void SetColor(const QColor &color);
  virtual void SetGraphNode (GraphNode *graphNode);
  virtual AnchorWidget *FindAnchorWidget (const QPointF &scenePos);

private:
  TextItem *m_text;
  FlowAnchorWidget *m_anchor;

  QString m_name;
};



class Headline : public QGraphicsWidget
{
  Q_OBJECT
public:
  Headline (QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());

  void SetLabel(const QString &label);
  void SetColor(const QColor &color);
  void AddFlowInItem();
  void AddFlowOutItem ();
  void SetGraphNode (GraphNode *graphNode);

  void Hover(const QPointF &scenePos);
  AnchorWidget *FindAnchorWidget (const QPointF &scenePos);

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
  QGraphicsGridLayout *m_layout;
  FlowAnchorWidget *m_flowIn;
  TextItem *m_text;
  FlowAnchorWidget *m_flowOut;

  GraphNode *m_graphNode;
  QPointF m_motionStartPosition;
  QColor m_color;
  QColor m_lowColor;
};

class GraphNode : public QGraphicsWidget
{
  Q_OBJECT
public:
  GraphNode (QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
  virtual ~GraphNode ();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;

  void Hover (const QPointF &scenePos);
  AnchorWidget *FindAnchorWidget (const QPointF &scenePos);

  void SetLabel(const QString &label);
  void SetHeadlineColor(const QColor &color);
  void AddInput (ContentWidget *input);
  void AddOutput(ContentWidget *output);
  ContentWidget *GetInput(int idx);
  ContentWidget *GetOutput(int idx);

  bool IsHeadLine(const QPointF &scenePos) const;
  void SetSelected (bool selected);
  bool IsSelected () const;


  void SetMotionStart(const QPointF &motionStart);
  const QPointF &GetMotionStart () const;

private:

  QList<ContentWidget*> m_contents;

  QGraphicsGridLayout *m_layout;
  Headline *m_headLine;

  int m_leftCount;
  QGraphicsWidget *m_left;
  QGraphicsGridLayout *m_leftLayout;

  QGraphicsWidget *m_centerFit;

  int m_rightCount;
  QGraphicsWidget *m_right;
  QGraphicsGridLayout *m_rightLayout;
  bool m_selected;

  QPointF m_motionStart;

};

}
