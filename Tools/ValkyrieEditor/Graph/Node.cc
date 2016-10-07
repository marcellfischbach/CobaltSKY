

#include <Graph/Node.hh>
#include <Graph/Connection.hh>
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

#define VK_MAX(a, b) ((a) > (b) ? (a) : (b))
#define VK_MIN(a, b) ((a) < (b) ? (a) : (b))

namespace graph
{

class NodeBackground : public QGraphicsRectItem
{
public:
  NodeBackground(Node *node, QGraphicsItem *item)
    : QGraphicsRectItem(item)
    , m_node(node)
    , m_inMotion(false)
  {

  }

  void mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::LeftButton && motionHandle.contains(event->pos()))
    {
      m_delta = mapToScene(QPointF(0, 0)) - mapToScene(event->pos());
      m_inMotion = true;
      if (m_node && m_node->GetScene())
      {
        m_node->GetScene()->SelectNode(m_node);
      }
    }
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::LeftButton)
    {
      m_inMotion = false;
    }
  }

  void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (m_inMotion)
    {
      QPointF pos = mapToScene(event->pos()) + m_delta;
      m_node->SetPosition(pos);
    }
  }
  QRectF motionHandle;

private:
  Node *m_node;
  QPointF m_delta;
  bool m_inMotion;

};


class NodeGroup : public QGraphicsItemGroup
{
public:
  NodeGroup(Node *node, QGraphicsItem *parent = 0)
    : QGraphicsItemGroup(parent)
  {
    setHandlesChildEvents(false);

    background = new NodeBackground(node, this);
    gradientHeight = 10.0f;
    color = QColor(128, 0, 0);
    borderWidth = 1.0f;
    borderColor = QColor(128, 128, 128);

    UpdateData();
  }

  void SetSize(float width, float height)
  {
    background->setRect(0, 0, width, height);
  }

  void SetTitleHeight(float titleHeight)
  {
    gradientHeight = titleHeight;
  }
  void SetColor(const QColor &color)
  {
    this->color = color;
  }

  void UpdateData()
  {
    background->setPen(QPen(QBrush(borderColor), borderWidth));

    QLinearGradient gradient(QPointF(0.0f, 0.0f), QPointF(0.0f, gradientHeight));
    gradient.setColorAt(0.0f, color);
    gradient.setColorAt(1.0f, QColor(32, 32, 32));

    background->setBrush(QBrush(gradient));
  }

  void SetSelected(bool selected)
  {
    if (selected)
    {
      borderColor = QColor(255, 255, 255);
      borderWidth = 2.0f;
    }
    else
    {
      borderColor = QColor(128, 128, 128);
      borderWidth = 1.0f;
    }

    UpdateData();
  }




  NodeBackground *background;
  float gradientHeight;
  QColor color;
  QColor borderColor;
  float borderWidth;
};


class NodeAnchor : public QGraphicsEllipseItem
{
public:
  NodeAnchor(Node *node, int idx, Direction dir, QGraphicsItem *parent, const QFont &fnt)
    : QGraphicsEllipseItem(parent)
    , m_connecting(false)
    , m_node(node)
    , m_dir(dir)
    , m_idx(idx)
  {
    QFontMetrics fm(fnt);
    setRect(0, 0, fm.height(), fm.height());
    setPen(QPen(QColor(255, 255, 255)));
    setBrush(QBrush(Qt::NoBrush));
  }

  static QRectF GetRect(const QFontMetrics &fm)
  {
    return QRectF(0, 0, fm.ascent(), fm.ascent());
  }

  void mousePressEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::LeftButton)
    {
      m_connecting = true;
    }
  }

  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::LeftButton && m_connecting)
    {
      NodeGraphScene *scn = static_cast<NodeGraphScene*>(scene());
      if (scn)
      {
        QRectF r0 = mapRectToScene(rect());
        QPointF p0 = r0.center();
        QPointF p1 = mapToScene(event->pos());
        scn->StopConnection(m_node, m_idx, m_dir, p0, p1);
      }
      m_connecting = false;
    }
  }

  void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (m_connecting)
    {
      NodeGraphScene *scn = static_cast<NodeGraphScene*>(scene());
      if (scn)
      {
        QRectF r0 = mapRectToScene(rect());
        QPointF p0 = r0.center();
        QPointF p1 = mapToScene(event->pos());
        scn->MoveConnection(m_node, m_idx, m_dir, p0, p1);
      }

    }
  }
private:
  bool m_connecting;
  Node *m_node;
  Direction m_dir;
  int m_idx;
};

class NodeLabel : public QGraphicsSimpleTextItem
{
public:
  NodeLabel(QGraphicsItem *parent, const QFont &fnt)
    : QGraphicsSimpleTextItem(parent)
  {
    setFont(fnt);
    SetColor(QColor(255, 255, 255));
  }

  void SetRect(const QRectF &rect)
  {
    m_rect = rect;
    Update();
  }

  void SetAlignment(int alignment)
  {
    m_alignment = alignment;
    Update();
  }

private:
  void Update()
  {
    float x = 0.0f;
    float y = 0.0f;
    QFontMetrics fm(font());
    QRect br = fm.boundingRect(text());

    if (m_alignment & Qt::AlignLeft)
    {
      x = m_rect.left() + br.left();
    }
    else if (m_alignment & Qt::AlignRight)
    {
      x = m_rect.right() - br.right();
    }
    else if (m_alignment & Qt::AlignHCenter)
    {
      x = (m_rect.left() + m_rect.right()) / 2.0f - (br.left() + br.right()) / 2.0f;
    }

    if (m_alignment & Qt::AlignTop)
    {
      y = m_rect.top() + br.top() + fm.ascent();
    }
    else if (m_alignment & Qt::AlignBottom)
    {
      y = m_rect.bottom() + br.top() - fm.descent();
    }
    else if (m_alignment & Qt::AlignCenter)
    {
      y = (m_rect.bottom() + m_rect.top()) / 2.0f + br.top() + fm.ascent() / 2.0f;
    }
    setPos(x, y);
  }

  void SetColor(const QColor &color)
  {
    m_color = color;
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(color));
  }


private:
  int m_alignment;
  QRectF m_rect;
  QColor m_color;

};

class NodeConstInput : public QGraphicsItemGroup
{
public:
  NodeConstInput(QGraphicsItem *parent, const QFont &fnt)
    : QGraphicsItemGroup(parent)
  {
    setHandlesChildEvents(false);

    m_background = new QGraphicsRectItem(this);
    m_background->setPen(QPen(QColor(0, 0, 0)));
    m_background->setBrush(QBrush(QColor(255, 255, 255)));

    m_textItem = new QGraphicsTextItem(this);
    m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);

    QFontMetrics fm(fnt);
    QRectF rect = GetRect(fm);
    m_background->setRect(rect);
    SetText("1.0");
    m_textItem->setTextWidth(rect.width());
  }

  void SetText(const QString &text)
  {
    m_textItem->setPlainText(text);
  }

  void SetConst(float value)
  {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, 256, "%f", value);
    for (int i = 255; i >= 0; --i)
    {
      if (buffer[i] == '\0' || buffer[i] == '0')
      {
        buffer[i] = '\0';
        continue;
      }
      if (buffer[i] == '.' && i != 255)
      {
        buffer[i + 1] = '0';
      }
      break;
    }
    SetText(QString(buffer));
  }

  float GetConst() const
  {
    QString text = m_textItem->toPlainText();
    return atof((const char*)text.toLatin1());
  }

  static QRectF GetRect(const QFontMetrics &fm)
  {
    return QRectF(0, 0, fm.width("GGGGGGG"), fm.height() *1.5);
  }

private:
  QGraphicsRectItem *m_background;
  QGraphicsTextItem *m_textItem;
};


class NodeNameInput : public QGraphicsItemGroup
{
public:
  NodeNameInput(QGraphicsItem *parent, const QFont &fnt)
    : QGraphicsItemGroup(parent)
  {
    setHandlesChildEvents(false);

    m_background = new QGraphicsRectItem(this);
    m_background->setPen(QPen(QColor(0, 0, 0)));
    m_background->setBrush(QBrush(QColor(255, 255, 255)));

    m_textItem = new QGraphicsTextItem(this);
    m_textItem->setTextInteractionFlags(Qt::TextEditorInteraction);

    QFontMetrics fm(fnt);
    QRectF rect = GetRect(fm);
    m_background->setRect(rect);
    SetText("");
  }

  QTextDocument *GetDocument()
  {
    return m_textItem->document();
  }

  void SetRect(const QRectF &rect)
  {
    m_background->setRect(rect);
    m_textItem->setPos(rect.topLeft());
    m_textItem->setTextWidth(rect.width());
  }

  void SetText(const QString &text)
  {
    m_textItem->setPlainText(text);
  }

  QString GetName() const
  {
    return m_textItem->toPlainText();
  }

  float GetHeight(const QFontMetrics &fm) const
  {

  }

  static QRectF GetRect(const QFontMetrics &fm)
  {
    return QRectF(0, 0, fm.width("GGGG"), fm.height() *1.5);
  }

private:
  QGraphicsRectItem *m_background;
  QGraphicsTextItem *m_textItem;
};

class NodeInputItem : public QGraphicsItemGroup
{
public:
  NodeInputItem(Node *node, int idx, Node::InputMode mode, const QString &labelText, const QFont &fnt, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
    , anchor(0)
    , constInput(0)
    , label(0)
  {
    setHandlesChildEvents(false);
    if (mode & Node::eIM_Output)
    {
      anchor = new NodeAnchor(node, idx, eD_Input, this, fnt);
    }
    if (mode & Node::eIM_Const)
    {
      constInput = new NodeConstInput(this, fnt);
    }

    label = new NodeLabel(this, fnt);
    label->setText(labelText);

  }

  void SetSizes(const QFontMetrics &fm, const QString &text, float anchorPos, float constPos, float labelPos)
  {
    if (anchor)
    {
      float height = (NodeConstInput::GetRect(fm).height() - NodeAnchor::GetRect(fm).height()) / 2.0f;
      anchor->setPos(anchorPos, height);
    }
    if (constInput)
    {
      constInput->setPos(constPos, 0);
    }
    QRectF rect(labelPos, 0.0f, fm.width(text), NodeConstInput::GetRect(fm).height());
    label->SetRect(rect);
    label->SetAlignment(Qt::AlignLeft | Qt::AlignCenter);
  }

  void SetConstVisible(bool visible)
  {
    if (constInput)
    {
      constInput->setVisible(visible);
    }
  }

  NodeAnchor *anchor;
  NodeConstInput *constInput;
  NodeLabel *label;
};

class NodeOutputItem : public QGraphicsItemGroup
{
public:
  NodeOutputItem(Node *node, int idx, const QString &labelText, const QFont &fnt, float spacing, QGraphicsItem *parent = 0)
    : QGraphicsItemGroup(parent)
  {
    setHandlesChildEvents(false);
    anchor = new NodeAnchor(node, idx, eD_Output, this, fnt);

    label = new NodeLabel(this, fnt);
    label->setFont(fnt);
    label->setText(labelText);

    QFontMetrics fm(fnt);
    QRectF anchorRect = NodeAnchor::GetRect(fm);
    float height = (NodeConstInput::GetRect(fm).height() - anchorRect.height()) / 2.0f;
    anchor->setPos(-anchorRect.width(), height);

    int textWidth = fm.width(labelText);

    QRectF rect(-textWidth - anchorRect.width() - spacing*2.0f, 0.0f, textWidth, NodeConstInput::GetRect(fm).height());
    label->SetRect(rect);
    label->SetAlignment(Qt::AlignRight | Qt::AlignCenter);


  }


  NodeAnchor *anchor;
  NodeLabel *label;
};




//Node *Node::selectedNode = 0;


Node::Node(QObject *parent)
  : QObject(parent)
  , m_scene(0)
  , m_color(128, 0, 0)
  , m_hasName(false)
  , m_minWidth(0.0f)
  , m_valid(false)
  , m_imageItem(0)
  , m_showImage(false)
{

}

/*
void Node::Select(Node *node)
{
  Node *current = Node::selectedNode;
  Node::selectedNode = node;

 
  if (node == current)
  {
    return;
  }

  if (current)
  {
    current->UpdateSelection();
  }
  if (node)
  {
    node->UpdateSelection();
  }
}

Node *Node::GetSelected()
{
  return Node::selectedNode;
}
*/

QString Node::GetName() const
{
  if (m_nodeNameInput)
  {
    return m_nodeNameInput->GetName();
  }
  return "";
}

void Node::SetImage(const QImage &image)
{
  if (!image.isNull() && (image.width() != 64 || image.height() != 64))
  {
    m_image = image.scaled(64, 64);
  }
  else
  {
    m_image = image;
  }
  if  (m_imageItem)
  {
    m_imageItem->setPixmap(QPixmap::fromImage(m_image));
  }
}

void Node::AddInput(const QString &label, const QString &key, InputMode mode)
{
  Input input;
  input.label = label;
  input.key = key;
  input.mode = mode;
  input.item = 0;

  m_inputs << input;
}

void Node::AddOutput(const QString &label, const QString &key)
{
  Output output;
  output.label = label;
  output.key = key;
  output.item = 0;

  m_outputs << output;
}

int Node::GetIndexOfInput(const QString &key) const
{
  for (int i = 0, in = m_inputs.size(); i < in; ++i)
  {
    if (m_inputs[i].key == key)
    {
      return i;
    }
  }
  return -1;
}

int Node::GetIndexOfOutput(const QString &key) const
{
  for (int i = 0, in = m_outputs.size(); i < in; ++i)
  {
    if (m_outputs[i].key == key)
    {
      return i;
    }
  }
  return -1;
}

QString Node::GetInputKey(int idx) const
{
  if (idx < 0 || idx >= m_inputs.size())
  {
    return "";
  }
  return m_inputs[idx].key;
}

QString Node::GetOutputKey(int idx) const
{
  if (idx < 0 || idx >= m_outputs.size())
  {
    return "";
  }
  return m_outputs[idx].key;
}


QPointF Node::GetAnchorInputPos(int idx) const
{
  if (idx < 0 || idx >= m_inputs.size())
  {
    return QPointF();
  }

  const Input &input = m_inputs[idx];
  if (input.item && input.item->anchor)
  {
    return input.item->anchor->sceneBoundingRect().center();
  }

  return QPointF();
}


QPointF Node::GetAnchorOutputPos(int idx) const
{
  if (idx < 0 || idx >= m_outputs.size())
  {
    return QPointF();
  }

  const Output &output = m_outputs[idx];
  if (output.item)
  {
    return output.item->anchor->sceneBoundingRect().center();
  }

  return QPointF();
}


size_t Node::GetNumberOfInputs() const
{
  return m_inputs.size();
}

float Node::GetConstInput(size_t idx) const
{
  if (idx >= m_inputs.size())
  {
    return 0.0f;
  }

  graph::NodeConstInput *constInput = m_inputs[(int)idx].item->constInput;
  if (!constInput)
  {
    return 0.0f;
  }

  return constInput->GetConst();
}

void Node::SetConstInput(size_t idx, float constValue)
{
  if (idx >= m_inputs.size())
  {
    return;
  }

  graph::NodeConstInput *constInput = m_inputs[(int)idx].item->constInput;
  if (!constInput)
  {
    return;
  }
  constInput->SetConst(constValue);

}

bool Node::Initialize()
{
  // first check any of the inputs need anchors and/or const inputs
  bool needAnchor = false;
  bool needConst = false;
  for (Input &input : m_inputs)
  {
    if (input.mode & eIM_Output)
    {
      needAnchor = true;
    }
    if (input.mode & eIM_Const)
    {
      needConst = true;
    }
  }

  // approximate the needed width and height

  QFont fnt;
  QFont fntTitle;
  fntTitle.setPixelSize(16);

  QFontMetrics fm(fnt);
  QFontMetrics fmTitle(fntTitle);

  float m_margin = 5.0f;
  float m_spacing = 5.0f;
  float m_middleSpacing = 15.0f;
  float titleHeight = fmTitle.height();

  float width = 0.0f;
  float height = 0.0f;

  float nameHeight = 0.0f;
  if (m_hasName)
  {
    nameHeight = fm.height() * 1.5f + m_spacing;
  }

  QRectF achorRect = NodeAnchor::GetRect(fm);
  QRectF constRect = NodeConstInput::GetRect(fm);

  // get the overall width of all inputs (this might include a possible image at the top)
  int inputWidth = 0;
  int inputHeight = (constRect.height() + m_spacing) * m_inputs.size();
  if (m_showImage)
  {
    // the images that are shown here will always be 64x64
    inputHeight += 64 + m_spacing;
    inputWidth += 64 + m_spacing;
  }
  for (Input &input : m_inputs)
  {
    inputWidth = VK_MAX(fm.width(input.label), inputWidth);
  }

  float inputAnchorPos = 0.0f;
  float inputConstPos = 0.0f;
  float inputLabelPos = 0.0f;

  if (needAnchor)
  {
    int anchorWidth = achorRect.width() + m_spacing;
    inputWidth += anchorWidth;
    inputConstPos += anchorWidth;
    inputLabelPos += anchorWidth;
  }
  if (needConst)
  {
    int constWidth = constRect.width() + m_spacing;
    inputWidth += constWidth;
    inputLabelPos += constWidth;
  }
  inputWidth += m_margin;


  // get the overall width of all outputs
  int outputWidth = 0;
  int outputHeight = (constRect.height() + m_spacing) * m_outputs.size();
  for (Output &output : m_outputs)
  {
    int width = fm.width(output.label);
    outputWidth = VK_MAX(outputWidth, width);
  }
  outputWidth += achorRect.width() + m_spacing;
  outputWidth += m_margin;



  // get the total width of the title
  int titleWidth = fmTitle.width(m_label) + 2.0f * m_margin;
  //  the execution path in and out (if needed) are added here later


  // now just get the final width;
  int inoutWidth = inputWidth + 2.0f * m_margin + outputWidth + m_middleSpacing;
  width = VK_MAX(titleWidth, inoutWidth);
  width = VK_MAX(width, m_minWidth);

  height = m_margin + titleHeight + nameHeight + VK_MAX(inputHeight, outputHeight) + m_margin;


  // create the parent object
  m_item = m_nodeGroup = new NodeGroup(this);
  m_nodeGroup->SetTitleHeight(titleHeight + 2.0f * m_margin);
  m_nodeGroup->SetColor(m_color);
  m_nodeGroup->UpdateData();
  m_nodeGroup->SetSize(width, height);
  m_nodeGroup->background->motionHandle = QRectF(0, 0, width, titleHeight + 2.0f * m_margin);



  int posY = m_margin + titleHeight + m_spacing;

  if (m_hasName)
  {
    float nameInputHeight = fm.height() * 1.5;
    m_nodeNameInput = new NodeNameInput(m_nodeGroup, fnt);
    m_nodeNameInput->SetRect(QRectF(m_margin, posY, width - 2.0f * m_margin, nameInputHeight));
    m_nodeNameInput->SetText(m_name);

    connect(m_nodeNameInput->GetDocument(), SIGNAL(contentsChanged()), this, SLOT(NameChanged()));

    posY += nameHeight;
  }

  int topPosY = posY;

  if (m_showImage)
  {
    m_imageItem = new QGraphicsPixmapItem(m_nodeGroup);
    if (!m_image.isNull())
    {
      m_imageItem->setPixmap(QPixmap::fromImage(m_image));
    }
    m_imageItem->setPos(m_margin, posY);

    posY += 64 + m_spacing;
  }


  // now place the inputs
  int i = 0;
  for (Input &input : m_inputs)
  {
    input.item = new NodeInputItem(this, i++, input.mode, input.label, fnt, m_nodeGroup);
    input.item->SetSizes(fm, input.label, inputAnchorPos, inputConstPos, inputLabelPos);
    input.item->setPos(m_margin, posY);

    posY += constRect.height() + m_spacing;
  }

  posY = topPosY;
  i = 0;
  for (Output &output : m_outputs)
  {
    output.item = new NodeOutputItem(this, i++, output.label, fnt, m_spacing, m_nodeGroup);
    output.item->setPos(width - m_margin, posY);

    posY += constRect.height() + m_spacing;
  }





  m_title = new NodeLabel(m_nodeGroup, fntTitle);
  m_title->setText(m_label);
  m_title->SetRect(QRect(0, 0, width, titleHeight));
  m_title->SetAlignment(Qt::AlignHCenter | Qt::AlignTop);

  m_valid = true;
  return true;
}

void Node::UpdateSelection()
{
  bool selected = m_scene && this == m_scene->GetSelectedNode();

  m_nodeGroup->SetSelected(selected);
}

void Node::ResetConstVisible()
{
  for (Input &input : m_inputs)
  {
    input.item->SetConstVisible(true);
  }
}

void Node::SetConstVisible(int input, bool visible)
{
  m_inputs[input].item->SetConstVisible(visible);
}


void Node::SetPosition(const QPointF &pos)
{
  m_item->setPos(pos);

  if (m_scene)
  {
    m_scene->NodeMoved(this);
  }

}

QVector<NodeConnection*> Node::GetAllConnections() const
{
  return m_connections;
}

void Node::AddConnection(NodeConnection *connection)
{
  m_connections.append(connection);
}

void Node::RemoveConnection(NodeConnection *connection)
{
  m_connections.removeAll(connection);
}

void Node::RemoveAllConnections()
{
}

bool Node::TestAnchor(const QPointF &pos, Node::AnchorRequestResult &result)
{
  int i = 0;
  for (Input &input : m_inputs)
  {
    if (input.item->anchor)
    {
      QRectF anchorRect = input.item->anchor->sceneBoundingRect();
      if (anchorRect.contains(pos))
      {
        result.idx = i;
        result.dir = eD_Input;
        result.key = input.key;
        result.pos = anchorRect.center();
        return true;
      }
    }
    i++;
  }

  i = 0;
  for (Output &output : m_outputs)
  {
    QRectF anchorRect = output.item->anchor->sceneBoundingRect();
    if (anchorRect.contains(pos))
    {
      result.idx = i;
      result.dir = eD_Output;
      result.key = output.key;
      result.pos = anchorRect.center();
      return true;
    }
    i++;
  }
  return false;
}

void Node::NameChanged()
{
  emit m_scene->NodeNameChanged(this);
}


TextItem::TextItem(QGraphicsItem *parent)
  : QGraphicsLayoutItem(), QGraphicsSimpleTextItem(parent)
{
}

TextItem::~TextItem()
{
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


FlowInOutItem::FlowInOutItem(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_connected (false)
{
}



FlowInOutItem::~FlowInOutItem()
{

}

QSizeF FlowInOutItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  return QSizeF(21.0f, 16.0f);
}

void FlowInOutItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  float scale = 4.0f;

  QPainterPath drawPath(QPointF(2.0f, 2.0f));
  drawPath.lineTo(13.0f, 2.0f);
  drawPath.lineTo(19.0f, 8.0f);
  drawPath.lineTo(13.0f, 14.0f);
  drawPath.lineTo(2.0f, 14.0f);
  drawPath.lineTo(2.0f, 2.0f);
  painter->setPen(QColor(255, 255, 255));
  painter->drawPath(drawPath);

  if (m_connected)
  {
    QPainterPath fillPath(QPointF(4.0f, 4.0f));
    fillPath.lineTo(12.0f, 4.0f);
    fillPath.lineTo(16.0f, 8.0f);
    fillPath.lineTo(12.0f, 12.0f);
    fillPath.lineTo(4.0f, 12.0f);
    fillPath.lineTo(4.0f, 4.0f);
    painter->fillPath(fillPath, QColor(255, 255, 255));
  }
}




AttribInOutItem::AttribInOutItem(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_connected (false)
  , m_visible(true)
{
}



AttribInOutItem::~AttribInOutItem()
{

}

QSizeF AttribInOutItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
  return QSizeF(16.0f, 16.0f);
}

void AttribInOutItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  if (!m_visible)
  {
    return;
  }

  QPainterPath drawPath;
  drawPath.addEllipse(QRectF(2.0f, 2.0f, 12.0f, 12.0f));
  painter->setPen(QColor(64, 64, 64));
  painter->drawPath(drawPath);

  if (m_connected)
  {
    QPainterPath fillPath;
    fillPath.addEllipse(QRectF(4.0f, 4.0f, 8.0f, 8.0f));
    painter->fillPath(fillPath, QColor(64, 64, 64));
  }
}


AttribInput::AttribInput(QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_name("Attribute")
  , m_value("10.0")
{
  QGraphicsGridLayout *layout = new QGraphicsGridLayout(this);
  layout->setContentsMargins(0.0f, 0.0f, 0.0f, 0.0f);

  m_anchor = new AttribInOutItem(this);
  m_text = new TextItem(this);
  m_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_text->setPen(QColor(64, 64, 64));

  layout->addItem(m_anchor, 0, 0, 1, 1);
  layout->addItem(m_text, 0, 1, 1, 1);

  UpdateText();
}

AttribInput::~AttribInput()
{

}

void AttribInput::SetName(const QString &name)
{
  m_name = name;
  UpdateText ();
}

void AttribInput::SetValue(const QString &value)
{
  m_value = value;
  UpdateText();
}

void AttribInput::UpdateText()
{
  QString text = "";
  if (m_value.length() != 0)
  {
    text += QString("[%1] ").arg(m_value);
  }
  text += m_name;
  m_text->setText(text);
}


Headline::Headline (QGraphicsItem *parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget(parent, wFlags)
  , m_flowIn(0)
  , m_flowOut(0)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

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
}

void Headline::AddFlowItem()
{
  m_flowIn = new FlowInOutItem(this);
  m_flowOut = new FlowInOutItem(this);

  m_layout->addItem(m_flowIn, 0, 0, 1, 1);
  m_layout->addItem(m_flowOut, 0, 2, 1, 1);
}

void Headline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QGraphicsWidget::paint(painter, option, widget);

  QLinearGradient lg (rect().left(), 0.0f, rect().right(), 0.0f);
  lg.setColorAt(0.0f, QColor(225, 0, 0));
  lg.setColorAt(1.0f, QColor(225, 128, 128));

  painter->fillRect(rect(), QBrush(lg));
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
  m_headLine->AddFlowItem();
  m_layout->addItem(m_headLine, 0, 0, 1, 2);

  m_leftCount = 0;
  m_left = new QGraphicsWidget(this);
  m_left->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  m_leftLayout = new QGraphicsGridLayout(m_left);
  m_leftLayout->setContentsMargins(0, 0, 0, 0);

  m_layout->addItem(m_left, 1, 0, 1, 1);

  m_rightCount = 0;
  m_right= new QGraphicsWidget(this);
  m_right->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  m_rightLayout = new QGraphicsGridLayout(m_right);
  m_rightLayout->setContentsMargins(0, 0, 0, 0);

  m_layout->addItem(m_right, 1, 1, 1, 1);

  m_layout->setColumnStretchFactor(0, 1);
  m_layout->setColumnStretchFactor(1, 1);

  AddInput(new AttribInput(this));
  AddInput(new AttribInput(this));
  FinishInput();
}

GraphNode::~GraphNode ()
{

}


void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QGraphicsWidget::paint(painter, option, widget);
  if (m_selected)
  {
    painter->fillRect(rect(), QColor(225, 225, 128));
  }

  painter->fillRect(m_layout->contentsRect(), QColor(225, 225, 225));
}


void GraphNode::AddInput(AttribInput *input)
{
  input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_leftLayout->addItem(input, m_leftCount, 0);
  m_leftCount++;
}

void GraphNode::FinishInput()
{
  QGraphicsWidget *widget = new QGraphicsWidget(this);
  widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_leftLayout->addItem(widget, m_leftCount, 0);
  m_leftCount++;
}

}


#undef VK_MAX
#undef VK_MIN
