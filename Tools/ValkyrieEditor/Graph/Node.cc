

#include <Graph/Node.hh>
#include <Graph/Scene.hh>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qbrush.h>
#include <qpen.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qtextdocument.h>
#include <QPainter>
#include <QGraphicsGridLayout>
#include <QDrag>
#include <QApplication>
#include <QMimeData>

#define VK_MAX(a, b) ((a) > (b) ? (a) : (b))
#define VK_MIN(a, b) ((a) < (b) ? (a) : (b))

namespace graph
{



TextItem::TextItem(QGraphicsItem *parent)
  : QGraphicsLayoutItem(), QGraphicsSimpleTextItem(parent)
  , m_graphNode(0)
{
}

TextItem::~TextItem()
{
}

void TextItem::SetColor(const QColor &color)
{
  QPen lpen(pen());
  lpen.setColor(color);
  setPen(lpen);
  update();
}

void TextItem::SetGraphNode(GraphNode *graphNode)
{
  m_graphNode = graphNode;
}

QRectF TextItem::boundingRect() const
{
  return QFontMetricsF(font()).boundingRect(text());
}

void TextItem::setGeometry(const QRectF &geom)
{
  prepareGeometryChange();
  QGraphicsLayoutItem::setGeometry(geom);
  setPos(geom.topLeft());
}

QSizeF TextItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  QRectF r = QFontMetricsF(font()).boundingRect(text());
  switch (which)
  {
  case Qt::MinimumSize:
  case Qt::PreferredSize:
    return QSizeF (r.width(), r.height());
  case Qt::MaximumSize:
    return QSizeF(FLT_MAX, r.height());
  }
  return QSizeF (r.width(), r.height());
}

SpacerItem::SpacerItem(QGraphicsLayoutItem *parent, int direction)
  : QGraphicsLayoutItem(parent)
{
  if (direction == 0)
  {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  }
  else
  {
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
  }
}


QSizeF SpacerItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  return QSizeF(1.0f, 1.0f);
}

AnchorConnectionItem::AnchorConnectionItem(QGraphicsItem *parent, AnchorWidget *anchorA, AnchorWidget *anchorB)
  : QObject()
  , QGraphicsPathItem(parent)
  , m_anchorA(anchorA)
  , m_anchorB(anchorB)
{
  if (m_anchorA)
  {
    m_anchorA->AddConnection(this);
    connect (m_anchorA->GetGraphNode(), SIGNAL(geometryChanged()), this, SLOT(AnchorChanged ()));
  }
  if (m_anchorB)
  {
    m_anchorB->AddConnection(this);
    connect (m_anchorB->GetGraphNode(), SIGNAL(geometryChanged()), this, SLOT(AnchorChanged ()));
  }

  QPen pen = QPen(QColor(225, 225, 225), 2.0f);
  setPen(pen);

  AnchorChanged();
}

AnchorConnectionItem::~AnchorConnectionItem()
{
  if (m_anchorA)
  {
    m_anchorA->RemoveConnection(this);
    m_anchorA = 0;
  }
  if (m_anchorB)
  {
    m_anchorB->RemoveConnection(this);
    m_anchorB = 0;
  }
}

AnchorWidget *AnchorConnectionItem::GetInputWidget()
{
  if (m_anchorA && m_anchorA->GetDirection() == AnchorWidget::eD_In)
  {
    return m_anchorA;
  }
  else if (m_anchorB && m_anchorB->GetDirection() == AnchorWidget::eD_In)
  {
    return m_anchorB;
  }
  return 0;
}


AnchorWidget *AnchorConnectionItem::GetOutputWidget()
{
  if (m_anchorA && m_anchorA->GetDirection() == AnchorWidget::eD_Out)
  {
    return m_anchorA;
  }
  else if (m_anchorB && m_anchorB->GetDirection() == AnchorWidget::eD_Out)
  {
    return m_anchorB;
  }
  return 0;
}


AnchorWidget *AnchorConnectionItem::GetAnchorA()
{
  return m_anchorA;
}

AnchorWidget *AnchorConnectionItem::GetAnchorB()
{
  return m_anchorB;
}

bool AnchorConnectionItem::IsEqual(AnchorWidget *anchorA, AnchorWidget *anchorB)
{
  return m_anchorA == anchorA && m_anchorB == anchorB ||
      m_anchorA == anchorB && m_anchorB == anchorA;
}

void AnchorConnectionItem::AnchorChanged()
{
  if (!m_anchorA || !m_anchorB)
  {
    return;
  }


  QPointF startPos = m_anchorA->mapToScene(m_anchorA->contentsRect().center());
  QPointF endPos = m_anchorB->mapToScene(m_anchorB->contentsRect().center());
  QPainterPath path;
  path.moveTo(startPos);

  float cx = (startPos.x() + endPos.x()) / 2.0f;
  float cy0 = startPos.y();
  float cy1 = endPos.y();
  path.cubicTo(QPointF(cx, cy0), QPointF(cx, cy1), endPos);
  setPath(path);
}




AnchorWidget::AnchorWidget(Type type, Direction direction, QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_type(type)
  , m_direction(direction)
  , m_connected (false)
  , m_hover(false)
  , m_graphNode(0)
  , m_visible(true)
{

}

AnchorWidget::~AnchorWidget()
{

}

void AnchorWidget::SetVisible(bool visible)
{
  m_visible = visible;
}

bool AnchorWidget::IsVisible() const
{
  return m_visible;
}

void AnchorWidget::Hover(const QPointF &scenePos)
{
  bool hover = mapRectToScene(contentsRect()).contains(scenePos);
  if (m_hover != hover)
  {
    m_hover = hover;
    update ();
  }
}


void AnchorWidget::AddConnection(AnchorConnectionItem *connection)
{
  if (m_connections.contains(connection))
  {
    return;
  }
  m_connections.append(connection);

  bool connected = !m_connections.isEmpty();
  if (m_connected != connected)
  {
    m_connected = connected;\
    update();
  }

  emit ConnectionChanged ();
}


void AnchorWidget::RemoveConnection(AnchorConnectionItem *connection)
{
  if (!m_connections.contains(connection))
  {
    return;
  }
  m_connections.removeAll(connection);


  bool connected = !m_connections.isEmpty();
  if (m_connected != connected)
  {
    m_connected = connected;\
    update();
  }

  emit ConnectionChanged ();

}



void AnchorWidget::SetGraphNode(GraphNode *graphNode)
{
  m_graphNode = graphNode;
}




AnchorWidget *AnchorWidget::FindAnchorWidget(const QPointF &scenePos)
{
  if (mapRectToScene(contentsRect()).contains(scenePos))
  {
    return this;
  }
  return 0;
}

AnchorWidget::Type AnchorWidget::GetType() const
{
  return m_type;
}


AnchorWidget::Direction AnchorWidget::GetDirection() const
{
  return m_direction;
}

GraphNode *AnchorWidget::GetGraphNode()
{
  return m_graphNode;
}

size_t AnchorWidget::GetNumberOfConnections() const
{
  return m_connections.size();
}

AnchorConnectionItem *AnchorWidget::GetConnection(size_t idx)
{
  return m_connections[idx];
}

bool AnchorWidget::IsConnected() const
{
  return m_connected;
}

void AnchorWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsWidget::mousePressEvent(event);
  if (m_graphNode)
  {
    NodeGraphScene *nodeGraphScene = static_cast<NodeGraphScene*>(m_graphNode->scene());
    nodeGraphScene->StartConnection(this);
    event->accept();
  }
}

void AnchorWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsWidget::mouseMoveEvent(event);

  if (m_graphNode)
  {
    NodeGraphScene *nodeGraphScene = static_cast<NodeGraphScene*>(m_graphNode->scene());
    nodeGraphScene->MoveConnection(event->scenePos());
  }
}

void AnchorWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsWidget::mouseReleaseEvent(event);

  if (m_graphNode)
  {
    NodeGraphScene *nodeGraphScene = static_cast<NodeGraphScene*>(m_graphNode->scene());
    nodeGraphScene->AttachConnect(event->scenePos());
  }
}






FlowAnchorWidget::FlowAnchorWidget(AnchorWidget::Direction direction, QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : AnchorWidget(eT_Flow, direction, parent, wFlags)
{
}



FlowAnchorWidget::~FlowAnchorWidget()
{

}


void FlowAnchorWidget::SetColor(const QColor &color)
{
  m_color = color;
}


QSizeF FlowAnchorWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  return QSizeF(21.0f, 16.0f);
}


void FlowAnchorWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  float scale = 4.0f;

  if (m_hover)
  {
    painter->fillRect(contentsRect(), QColor(128, 128, 128));
  }

  QPainterPath drawPath(QPointF(2.0f, 2.0f));
  drawPath.lineTo(13.0f, 2.0f);
  drawPath.lineTo(19.0f, 8.0f);
  drawPath.lineTo(13.0f, 14.0f);
  drawPath.lineTo(2.0f, 14.0f);
  drawPath.lineTo(2.0f, 2.0f);
  painter->setPen(m_color);
  painter->drawPath(drawPath);

  if (m_connected)
  {
    QPainterPath fillPath(QPointF(4.0f, 4.0f));
    fillPath.lineTo(12.0f, 4.0f);
    fillPath.lineTo(16.0f, 8.0f);
    fillPath.lineTo(12.0f, 12.0f);
    fillPath.lineTo(4.0f, 12.0f);
    fillPath.lineTo(4.0f, 4.0f);
    painter->fillPath(fillPath, m_color);
  }
}




AttribAnchorWidget::AttribAnchorWidget(Direction direction, QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : AnchorWidget(eT_Attrib, direction, parent, wFlags)
  , m_color(64, 64, 64)
{
  setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}



AttribAnchorWidget::~AttribAnchorWidget()
{

}

void AttribAnchorWidget::SetColor(const QColor &color)
{
  m_color = color;
}


QSizeF AttribAnchorWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  return QSizeF(16.0f, 16.0f);
}

void AttribAnchorWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  if (!m_visible)
  {
    return;
  }

  if (m_hover)
  {
    painter->fillRect(contentsRect(), QColor(128, 128, 128));
  }

  QPainterPath drawPath;
  drawPath.addEllipse(QRectF(2.0f, 2.0f, 12.0f, 12.0f));
  painter->setPen(m_color);
  painter->drawPath(drawPath);

  if (m_connected)
  {
    QPainterPath fillPath;
    fillPath.addEllipse(QRectF(4.0f, 4.0f, 8.0f, 8.0f));
    painter->fillPath(fillPath, m_color);
  }
}

ContentWidget::ContentWidget(Type type, Mode mode, QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_graphNode(0)
  , m_index(-1)
  , m_type(type)
  , m_mode(mode)
{

}

ContentWidget::~ContentWidget()
{

}

void ContentWidget::SetGraphNode(GraphNode *graphNode)
{
  m_graphNode = graphNode;
}


void ContentWidget::SetIndex(int index)
{
  m_index = index;
}

int ContentWidget::GetIndex() const
{
  return m_index;
}

ContentWidget::Type ContentWidget::GetType() const
{
  return m_type;
}


ContentWidget::Mode ContentWidget::GetMode() const
{
  return m_mode;
}


AttribInputWidget::AttribInputWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : ContentWidget(eT_Attribute, eM_Input, parent, wFlags)
  , m_name("Attribute")
  , m_value("")
{
  QGraphicsGridLayout *layout = new QGraphicsGridLayout(this);
  layout->setContentsMargins(0.0f, 0.0f, 0.0f, 0.0f);

  m_anchor = new AttribAnchorWidget(AnchorWidget::eD_In, this);
  m_text = new TextItem(this);
  m_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_text->SetColor(QColor(64, 64, 64));

  SpacerItem *spacer = new SpacerItem(this, 0);

  UpdateText();

  layout->addItem(m_anchor, 0, 0, 1, 1);
  layout->addItem(m_text, 0, 1, 1, 1, Qt::AlignLeft);
  layout->addItem(spacer, 0, 2, 1, 1);

  layout->setColumnStretchFactor(0, 0);
  layout->setColumnStretchFactor(1, 0);
  layout->setColumnStretchFactor(2, 1);

  connect (m_anchor, SIGNAL(ConnectionChanged()), this, SLOT(AnchorConnectionChanged ()));

}


AttribInputWidget::~AttribInputWidget()
{

}

void AttribInputWidget::SetName(const QString &name)
{
  m_name = name;
  UpdateText ();
}

void AttribInputWidget::SetValue(const QString &value)
{
  m_value = value;
  UpdateText();
}

void AttribInputWidget::SetColor(const QColor &color)
{
  m_text->SetColor(color);
  m_anchor->SetColor(color);
}

void AttribInputWidget::UpdateText()
{
  QString text = "";
  if (m_value.length() != 0 && (!m_anchor || !m_anchor->IsConnected()))
  {
    text += QString("[%1] ").arg(m_value);
  }
  text += m_name;
  m_text->setText(text);
  m_text->update();
}

void AttribInputWidget::AnchorConnectionChanged()
{
  UpdateText ();
}


void AttribInputWidget::SetGraphNode(GraphNode *graphNode)
{
  ContentWidget::SetGraphNode(graphNode);
  m_text->SetGraphNode(graphNode);
  m_anchor->SetGraphNode(graphNode);
}

void AttribInputWidget::Hover(const QPointF &scenePos)
{
  if (m_anchor)
  {
    m_anchor->Hover(scenePos);
  }
}


AnchorWidget *AttribInputWidget::FindAnchorWidget (const QPointF &scenePos)
{
  if (m_anchor)
  {
    return m_anchor->FindAnchorWidget(scenePos);
  }
  return 0;
}

AttribAnchorWidget *AttribInputWidget::GetAnchor()
{
  return m_anchor;
}


AttribOutputWidget::AttribOutputWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : ContentWidget(eT_Attribute, eM_Output, parent, wFlags)
  , m_name("Attribute")
{
  QGraphicsGridLayout *layout = new QGraphicsGridLayout(this);
  layout->setContentsMargins(0.0f, 0.0f, 0.0f, 0.0f);

  m_text = new TextItem(this);
  m_text->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_text->SetColor(QColor(64, 64, 64));
  m_text->setText(m_name);
  m_anchor = new AttribAnchorWidget(AnchorWidget::eD_Out, this);


  layout->addItem(m_text, 0, 0, Qt::AlignRight);
  layout->addItem(m_anchor, 0, 1);

}

AttribOutputWidget::~AttribOutputWidget()
{

}

void AttribOutputWidget::SetName(const QString &name)
{
  m_name = name;
  m_text->setText(m_name);
}


void AttribOutputWidget::SetColor(const QColor &color)
{
  m_text->SetColor(color);
  m_anchor->SetColor(color);
}

void AttribOutputWidget::SetGraphNode(GraphNode *graphNode)
{
  ContentWidget::SetGraphNode(graphNode);
  m_text->SetGraphNode(graphNode);
  m_anchor->SetGraphNode(graphNode);
}

void AttribOutputWidget::Hover(const QPointF &scenePos)
{
  if (m_anchor)
  {
    m_anchor->Hover(scenePos);
  }

}

AnchorWidget *AttribOutputWidget::FindAnchorWidget (const QPointF &scenePos)
{
  if (m_anchor)
  {
    return m_anchor->FindAnchorWidget(scenePos);
  }
  return 0;
}




AttribAnchorWidget *AttribOutputWidget::GetAnchor()
{
  return m_anchor;
}

FlowOutputWidget::FlowOutputWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : ContentWidget(eT_Flow, eM_Output, parent, wFlags)
  , m_name("Attribute")
{
  QGraphicsGridLayout *layout = new QGraphicsGridLayout(this);
  layout->setContentsMargins(0.0f, 0.0f, 0.0f, 0.0f);

  m_text = new TextItem(this);
  m_text->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_text->SetColor(QColor(64, 64, 64));
  m_text->setText(m_name);
  m_anchor = new FlowAnchorWidget(AnchorWidget::eD_Out, this);


  layout->addItem(m_text, 0, 0, Qt::AlignRight);
  layout->addItem(m_anchor, 0, 1);

}

FlowOutputWidget::~FlowOutputWidget()
{

}

void FlowOutputWidget::SetName(const QString &name)
{
  m_name = name;
  m_text->setText(m_name);
}


void FlowOutputWidget::SetGraphNode(GraphNode *graphNode)
{
  ContentWidget::SetGraphNode(graphNode);
  m_text->SetGraphNode(graphNode);
  m_anchor->SetGraphNode(graphNode);
}

void FlowOutputWidget::Hover(const QPointF &scenePos)
{
  if (m_anchor)
  {
    m_anchor->Hover(scenePos);
  }

}

void FlowOutputWidget::SetColor(const QColor &color)
{
  m_anchor->SetColor(color);
}

AnchorWidget *FlowOutputWidget::FindAnchorWidget (const QPointF &scenePos)
{
  if (m_anchor)
  {
    return m_anchor->FindAnchorWidget(scenePos);
  }
  return 0;
}



Headline::Headline (QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_flowIn(0)
  , m_flowOut(0)
  , m_color(0, 0, 0)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  setAcceptedMouseButtons(Qt::LeftButton);

  m_text = new TextItem(this);
  m_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_text->setText("Test Node");


  m_layout = new QGraphicsGridLayout(this);
  m_layout->setContentsMargins(2.0, 2.0, 2.0, 2.0);
  m_layout->setHorizontalSpacing(6.0);
  m_layout->addItem(m_text, 0, 1, 1, 1, Qt::AlignVCenter);

  m_layout->setColumnStretchFactor(0, 0);
  m_layout->setColumnStretchFactor(1, 1);
  m_layout->setColumnStretchFactor(2, 0);
  SetColor(QColor(255, 0, 0));
}

void Headline::SetLabel(const QString &label)
{
  m_text->setText(label);
}

void Headline::SetColor(const QColor &color)
{
  m_color = color;
  m_lowColor = m_color.toHsv();
  m_lowColor.setHsv(m_lowColor.hsvHue(), m_lowColor.hsvSaturation(), m_lowColor.value() * 0.8);
}

void Headline::AddFlowInItem()
{
  m_flowIn = new FlowAnchorWidget(AnchorWidget::eD_In, this);
  m_flowIn->SetGraphNode(m_graphNode);

  m_layout->addItem(m_flowIn, 0, 0, 1, 1);
}

void Headline::AddFlowOutItem()
{
  m_flowOut = new FlowAnchorWidget(AnchorWidget::eD_Out, this);
  m_flowOut->SetGraphNode(m_graphNode);

  m_layout->addItem(m_flowOut, 0, 2, 1, 1);
}

void Headline::SetGraphNode(GraphNode *graphNode)
{
  m_graphNode = graphNode;
  m_text->SetGraphNode(graphNode);
  if (m_flowIn)
  {
    m_flowIn->SetGraphNode(graphNode);
  }
  if (m_flowOut)
  {
    m_flowOut->SetGraphNode(graphNode);
  }
}

void Headline::Hover(const QPointF &scenePos)
{
  if (m_flowIn)
  {
    m_flowIn->Hover(scenePos);
  }
  if (m_flowOut)
  {
    m_flowOut->Hover(scenePos);
  }
}

AnchorWidget *Headline::FindAnchorWidget (const QPointF &scenePos)
{
  if (m_flowIn)
  {
    AnchorWidget *item = m_flowIn->FindAnchorWidget(scenePos);
    if (item)
    {
      return item;
    }
  }
  if (m_flowOut)
  {
    AnchorWidget *item = m_flowOut->FindAnchorWidget(scenePos);
    if (item)
    {
      return item;
    }
  }
  return 0;
}


void Headline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QGraphicsWidget::paint(painter, option, widget);

  QLinearGradient lg (rect().left(), 0.0f, rect().right(), 0.0f);
  lg.setColorAt(0.0f, m_color);
  lg.setColorAt(1.0f, m_lowColor);

  painter->fillRect(rect(), QBrush(lg));
}

void Headline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsWidget::mousePressEvent(event);
  event->accept();

  graph::NodeGraphScene *nodeGraphScene = static_cast<graph::NodeGraphScene*>(scene());
  nodeGraphScene->StartMoving();
  if (m_graphNode)
  {
    nodeGraphScene->SelectNode(m_graphNode, event->modifiers());
  }


  m_motionStartPosition = event->scenePos();
}

void Headline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsWidget::mouseMoveEvent(event);

  QPointF currentPos = event->scenePos();
  QPointF direction = currentPos - m_motionStartPosition;

  graph::NodeGraphScene *nodeGraphScene = static_cast<graph::NodeGraphScene*>(scene());
  nodeGraphScene->MoveSelectedNodes(direction);

}


void Headline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsWidget::mouseReleaseEvent(event);
}


GraphNode::GraphNode(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_selected(false)
{

  m_layout = new QGraphicsGridLayout(this);
  m_layout->setHorizontalSpacing(0);
  m_layout->setVerticalSpacing(0);
  m_layout->setContentsMargins(2.0f, 2.0f, 2.0f, 2.0f);


  m_headLine = new Headline(this);
  m_headLine->SetGraphNode(this);
  m_layout->addItem(m_headLine, 0, 0, 1, 3);

  m_leftCount = 0;
  m_left = new QGraphicsWidget(this);
  m_left->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  m_leftLayout = new QGraphicsGridLayout(m_left);
  m_leftLayout->setContentsMargins(0, 0, 0, 0);

  m_layout->addItem(m_left, 1, 0);

  QGraphicsWidget *center = new QGraphicsWidget(this);
  center->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  m_layout->addItem(center, 1, 1);

  m_rightCount = 0;
  m_right= new QGraphicsWidget(this);
  m_right->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  m_rightLayout = new QGraphicsGridLayout(m_right);
  m_rightLayout->setContentsMargins(0, 0, 0, 0);

  m_layout->addItem(m_right, 1, 2);




}

GraphNode::~GraphNode ()
{

}

void GraphNode::SetSelected(bool selected)
{
  m_selected = selected;
  update();
}

bool GraphNode::IsSelected() const
{
  return m_selected;
}

void GraphNode::SetMotionStart(const QPointF &motionStart)
{
  m_motionStart = motionStart;
}

const QPointF &GraphNode::GetMotionStart() const
{
  return m_motionStart;
}


void GraphNode::Hover(const QPointF &scenePos)
{
  m_headLine->Hover(scenePos);
  for (auto content : m_contents)
  {
    content->Hover (scenePos);
  }
}

AnchorWidget *GraphNode::FindAnchorWidget(const QPointF &scenePos)
{
  for (auto content : m_contents)
  {
    AnchorWidget *item = content->FindAnchorWidget(scenePos);
    if (item)
    {
      return item;
    }
  }
  AnchorWidget *item = m_headLine->FindAnchorWidget(scenePos);
  if (item)
  {
    return item;
  }

  return 0;
}



void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QGraphicsWidget::paint(painter, option, widget);
  if (m_selected)
  {
    painter->fillRect(rect(), QColor(255, 255, 255));
  }

  painter->fillRect(m_layout->contentsRect(), QColor(225, 225, 225));
}

void GraphNode::SetLabel(const QString &label)
{
  if (m_headLine)
  {
    m_headLine->SetLabel(label);
  }
}

void GraphNode::SetHeadlineColor(const QColor &color)
{
  if (m_headLine)
  {
    m_headLine->SetColor(color);
  }
}

void GraphNode::AddInput(ContentWidget *input)
{
  m_contents.append(input);
  input->SetGraphNode(this);
  m_leftLayout->addItem(input, m_leftCount, 0);
  m_leftCount++;
}

void GraphNode::AddOutput(ContentWidget *output)
{
  m_contents.append(output);
  output->SetGraphNode(this);
  m_rightLayout->addItem(output, m_rightCount, 0, Qt::AlignRight);
  m_rightCount++;
}

ContentWidget *GraphNode::GetInput(int idx)
{
  for (auto content : m_contents)
  {
    if (content->GetMode() == ContentWidget::eM_Input && content->GetIndex() == idx)
    {
      return content;
    }
  }
  return 0;
}


ContentWidget *GraphNode::GetOutput(int idx)
{
  for (auto content : m_contents)
  {
    if (content->GetMode() == ContentWidget::eM_Output && content->GetIndex() == idx)
    {
      return content;
    }
  }
  return 0;
}


bool GraphNode::IsHeadLine(const QPointF &scenePos) const
{
  return m_headLine->sceneBoundingRect().contains(scenePos);
}

}


#undef VK_MAX
#undef VK_MIN
