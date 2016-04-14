
#include <ShaderGraph/Node.hh>
#include <ShaderGraph/Connection.hh>
#include <qbrush.h>
#include <qpen.h>
#include <qpainter.h>
#include <qgraphicsitem.h>
#include <qgraphicssceneevent.h>
#include <qtextdocument.h>

ShaderGraphNode *ShaderGraphNode::currentSelectedNode = 0;

class ShaderGraphNodeGroup : public QGraphicsItemGroup
{
public:
  ShaderGraphNodeGroup (ShaderGraphNode *node)
    : QGraphicsItemGroup()
    , m_inMotion(false)
    , m_node(node)
  {

  }

  virtual void mousePressEvent (QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::LeftButton && event->pos().y() <= m_handleSize)
    {
      event->accept();
      m_startMouseScenePos = mapToScene(event->pos());
      m_startGroupScenePos = mapToScene(0, 0);
      m_inMotion = true;
      grabMouse();
      ShaderGraphNode::Select(m_node);
    }
    else
    {
//      QGraphicsItemGroup::mousePressEvent(event);
    }
  }

  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    m_inMotion = false;
    ungrabMouse();
  }

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (m_inMotion)
    {
      QPointF currentScenePos = mapToScene (event->pos());
      QPointF delta = currentScenePos - m_startMouseScenePos;
      QPointF newPos = m_startGroupScenePos + delta;
      setPos(newPos);
      m_node->UpdateConnections ();
    }
  }

  void SetHandleSize (float handleSize)
  {
    m_handleSize = handleSize;
  }

private:
  float m_handleSize;
  bool m_inMotion;
  ShaderGraphNode *m_node;
  QPointF m_startMouseScenePos;
  QPointF m_startGroupScenePos;
};

class ShaderGraphNodeAnchor : public QGraphicsEllipseItem
{
public:
  ShaderGraphNodeAnchor (QGraphicsItem *parent, ShaderGraphNode *node, bool isInput, int index)
    : QGraphicsEllipseItem(parent)
    , m_node (node)
    , m_moving (false)
    , m_isInput(isInput)
    , m_index(index)
  {

  }

  virtual void mousePressEvent (QGraphicsSceneMouseEvent *event)
  {
    if (event->button() == Qt::LeftButton)
    {
      m_moving = true;
    }

  }

  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
  {
    m_moving = false;
    QPointF c = mapToScene(boundingRect().center());
    QPointF p = mapToScene(event->pos());
    m_node->onStopDrag(m_isInput, m_index, c, p);
  }

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
  {
    if (m_moving)
    {
      QPointF c = mapToScene(boundingRect().center());
      QPointF p = mapToScene(event->pos());
      m_node->onMoveDrag(m_isInput, m_index, c, p);
    }
  }

  bool contains (const QPointF &point, QPointF &out) const
  {
    QPointF p = mapFromScene(point);
    if (boundingRect().contains(p))
    {
      out = mapToScene(boundingRect().center());
      return true;
    }
    return false;
  }

private:
  ShaderGraphNode *m_node;
  bool m_isInput;
  int m_index;
  bool m_moving;
};



ShaderGraphNode::ShaderGraphNode(vkSGNode *node)
  : QObject()
  , m_node(node)
  , m_titleFontSize(12)
  , m_inoutFontSize(11)
  , m_selected (false)
{

  QFont fntTitle;
  fntTitle.setPixelSize(m_titleFontSize);
  QFontMetrics fmTitle (fntTitle);

  // the interface font (interface => inputs and outputs)
  QFont fntIF;
  fntIF.setPixelSize(m_inoutFontSize);
  QFontMetrics fmIF(fntIF);

  QString nodeTitle(node->GetName().c_str());

  int margin = 5;
  int spacing = 3;
  int constInputWidth = fmIF.width("GGGG");
  int constInputHeight = fmIF.height() + fmIF.descent();
  int elipseSize = fmIF.height() ;
  int titleWidth = fmTitle.width(nodeTitle) + 2*margin;


  m_parent = new ShaderGraphNodeGroup(this);
  m_parent->setHandlesChildEvents(false);
  m_parent->setFlag(QGraphicsItem::ItemIsMovable, false);


  m_background = new QGraphicsRectItem();
  m_background->setPen(QPen(QColor(255, 255, 255)));
  m_background->setBrush(QBrush(QColor(64, 64, 64)));
//  m_background->setAcceptedMouseButtons(0);
  m_parent->addToGroup(m_background);

  m_title = new QGraphicsSimpleTextItem (m_background);
  m_title->setFont(fntTitle);
  m_title->setText(nodeTitle);
  m_title->setPen(QPen(Qt::NoPen));
  m_title->setBrush(QBrush(QColor(255, 255, 255)));
  m_title->setPos(margin, margin);
  m_parent->addToGroup(m_title);

  int titleHeight = fmTitle.height() + 2*spacing + margin ;
  int posInY = titleHeight;

  m_parent->SetHandleSize(titleHeight);


  int inputWidth = 0;
  int inputHeight = 0;
  int maxAnchorWidth = 0;
  int maxAnchorNameWidth = 0;
  for (size_t i=0, in=m_node->GetNumberOfInputs(); i<in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    QString inputName (input->GetName().c_str());

    int thisWidth = margin + elipseSize + spacing;

    Anchor anchor;
    memset(&anchor, 0, sizeof(Anchor));
    if (input->CanInputNode())
    {
      anchor.anchor = new ShaderGraphNodeAnchor(m_background, this, true, i);
      anchor.anchor->setRect(margin, posInY, elipseSize, elipseSize);
      anchor.anchor->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));
      anchor.anchor->setBrush(QBrush(Qt::NoBrush));
      m_parent->addToGroup(anchor.anchor);
      maxAnchorWidth = elipseSize + spacing;
    }
    anchor.text = new QGraphicsSimpleTextItem(m_background);
    anchor.text->setFont(fntIF);
    anchor.text->setText(inputName);
    anchor.text->setPos(thisWidth, posInY);
    anchor.text->setPen(QPen(Qt::NoPen));
    anchor.text->setBrush(QBrush(QColor(255, 255, 255)));
    m_parent->addToGroup(anchor.text);

    maxAnchorNameWidth = maxAnchorNameWidth < fmIF.width(inputName) ? fmIF.width(inputName) : maxAnchorNameWidth;

    thisWidth += fmIF.width(inputName);
    if (input->CanInputConst())
    {
      thisWidth += spacing;
      anchor.editValueBackground = new QGraphicsRectItem(m_background);
      anchor.editValueBackground->setPen(QPen(QColor(0, 0, 0)));
      anchor.editValueBackground->setBrush(QBrush(QColor(255, 255, 255)));
      anchor.editValueBackground->setRect(thisWidth, posInY, constInputWidth, constInputHeight);

      anchor.editValue = new QGraphicsTextItem(anchor.editValueBackground);
      anchor.editValue->setPos(thisWidth, posInY);
      anchor.editValue->setPlainText("1");
      anchor.editValue->setTextInteractionFlags(Qt::TextEditorInteraction);
      anchor.editValue->setTextWidth(constInputWidth);
      anchor.editValue->setFont(fntIF);

      m_parent->addToGroup(anchor.editValueBackground);
      m_parent->addToGroup(anchor.editValue);

      thisWidth += constInputWidth;
      posInY += fmIF.descent();
    }

    // add a margin on the far side
    thisWidth += margin;

    inputWidth = inputWidth < thisWidth ? thisWidth : inputWidth;

    posInY += fmIF.height() + spacing;
    inputHeight += fmIF.height() + spacing;
    m_inputs.append(anchor);
  }
  maxAnchorNameWidth += spacing;

  printf ("Max Values: %d %d\n", maxAnchorWidth, maxAnchorNameWidth);
  fflush(stdout);

  for (int i=0; i<m_inputs.size(); ++i)
  {
    Anchor &anchor = m_inputs[i];
    anchor.text->setPos(margin + maxAnchorWidth, anchor.text->pos().y());
    if (anchor.editValue && anchor.editValueBackground)
    {
      QRectF rect = anchor.editValueBackground->rect();
      rect.setX(margin + maxAnchorWidth + maxAnchorNameWidth);
      rect.setWidth(constInputWidth);
      anchor.editValueBackground->setRect(rect);
      anchor.editValue->setPos(margin + maxAnchorWidth + maxAnchorNameWidth, anchor.editValue->pos().y());
    }
  }



  int outputWidth = 0;
  for (size_t i=0, in=m_node->GetNumberOfOutputs(); i<in; ++i)
  {
    vkSGOutput *output = m_node->GetOutput(i);
    QString outputName (output->GetName().c_str());
    int thisWidth = margin + elipseSize + spacing + fmIF.width(outputName) + margin;
    outputWidth = outputWidth < thisWidth ? thisWidth : outputWidth;
  }

  int inOutWidth = inputWidth + outputWidth;
  int width = inOutWidth > titleWidth ? inOutWidth : titleWidth;

  int posOutY = titleHeight;
  for (size_t i=0, in=m_node->GetNumberOfOutputs(); i<in; ++i)
  {
    vkSGOutput *output = m_node->GetOutput(i);
    QString outputName (output->GetName().c_str());

    Anchor anchor;
    memset(&anchor, 0, sizeof(Anchor));

    anchor.anchor = new ShaderGraphNodeAnchor(m_background, this, false, i);
    anchor.anchor->setRect(width - margin - elipseSize, posOutY, elipseSize, elipseSize);
    anchor.anchor->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));
    anchor.anchor->setBrush(QBrush(Qt::NoBrush));
    m_parent->addToGroup(anchor.anchor);

    int textWidth = fmIF.width(outputName);

    anchor.text = new QGraphicsSimpleTextItem(m_background);
    anchor.text->setFont(fntIF);
    anchor.text->setText(outputName);
    anchor.text->setPos(width - margin - elipseSize - spacing - textWidth, posOutY);
    anchor.text->setPen(QPen(Qt::NoPen));
    anchor.text->setBrush(QBrush(QColor(255, 255, 255)));
    m_parent->addToGroup(anchor.text);

    int thisWidth = margin + elipseSize + spacing + textWidth + margin;
    outputWidth = outputWidth < thisWidth ? thisWidth : outputWidth;
    posOutY += fmIF.height() + spacing;

    m_outputs.append(anchor);
  }

  int height = posInY > posOutY ? posInY : posOutY;

  m_background->setRect(0, 0, width, height);


}

void ShaderGraphNode::SetSelected(bool selected)
{
  if (selected)
  {
    m_background->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));
  }
  else
  {
    m_background->setPen(QPen(QBrush(QColor(128, 128, 128)), 1.0f));
  }
}

QGraphicsItem *ShaderGraphNode::GetItem()
{
  return m_parent;
}

QPointF ShaderGraphNode::GetInputAnchor(int idx) const
{
  const Anchor &anchor = m_inputs[idx];
  return anchor.anchor->mapToScene(anchor.anchor->boundingRect().center());
}


QPointF ShaderGraphNode::GetOutputAnchor(int idx) const
{
  const Anchor &anchor = m_outputs[idx];
  return anchor.anchor->mapToScene(anchor.anchor->boundingRect().center());
}

void ShaderGraphNode::UpdateConnections()
{
  for (size_t i=0, in=m_inputs.size(); i<in; ++i)
  {
    Anchor &anchor = m_inputs[i];
    if (anchor.connection)
    {
      anchor.connection->UpdateValues();
    }
  }
  for (size_t i=0, in=m_outputs.size(); i<in; ++i)
  {
    Anchor &anchor = m_outputs[i];
    if (anchor.connection)
    {
      anchor.connection->UpdateValues();
    }
  }

}

void ShaderGraphNode::ConnectInput(ShaderGraphConnection *connection, int idx)
{
  m_inputs[idx].connection = connection;
}

void ShaderGraphNode::ConnectOutput(ShaderGraphConnection *connection, int idx)
{
  m_outputs[idx].connection = connection;
}


bool ShaderGraphNode::hasAnchor(bool input, const QPointF &globalP, QPointF &globalOut, int &index) const
{
  if (input)
  {
    for (size_t i=0, in=m_inputs.size(); i<in; ++i)
    {
      const Anchor &anchor = m_inputs[i];
      if (anchor.anchor)
      {
        if (anchor.anchor->contains(globalP, globalOut))
        {
          index = i;
          return true;
        }
      }
    }
  }
  else
  {
    for (size_t i=0, in=m_inputs.size(); i<in; ++i)
    {
      const Anchor &anchor = m_inputs[i];
      if (anchor.anchor)
      {
        if (anchor.anchor->contains(globalP, globalOut))
        {
          index = i;
          return true;
        }
      }
    }
  }
  return false;
}

void ShaderGraphNode::onMoveDrag(bool input, int index, const QPointF &pointA, const QPointF &pointB)
{
  emit moveDrag(input, index, pointA, pointB);
}

void ShaderGraphNode::onStopDrag(bool input, int index, const QPointF &pointA, const QPointF &pointB)
{
  emit stopDrag(input, index, pointA, pointB);
}


void ShaderGraphNode::Select(ShaderGraphNode *node)
{
  if (ShaderGraphNode::currentSelectedNode)
  {
    ShaderGraphNode::currentSelectedNode->SetSelected(false);
  }
  ShaderGraphNode::currentSelectedNode = node;
  if (ShaderGraphNode::currentSelectedNode)
  {
    ShaderGraphNode::currentSelectedNode->SetSelected(true);
  }
}
