
#include <ShaderGraph/Node.hh>
#include <ShaderGraph/Connection.hh>
#include <ShaderGraph/NodeSelector.hh>
#include <ShaderGraph/ShaderGraphView.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>
#include <qevent.h>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>

class GraphNode : public QGraphicsRectItem
{
public:
  GraphNode(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
  {
    setRect(-50.0f, -50.0f, 100.0f, 100.0f);
    setBrush(QBrush(QColor(128, 128, 128)));
    setPen(QPen(QColor(0, 0, 0)));
  }
};


ShaderGraphView::ShaderGraphView(QWidget *parent)
  : QWidget(parent)
  , m_currentBounding(0)
{
  setMouseTracking(true);
  m_view = new QGraphicsView(this);
  m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_view);


  m_view->setBackgroundBrush(QBrush(QColor(32, 32, 32)));


  m_scene = new QGraphicsScene();
  m_view->setScene(m_scene);

  //addNode(vkSGFloat4::GetStaticClass());



  /*
  QGraphicsPathItem *path = new QGraphicsPathItem();
  QPen pen(QColor(255, 255, 255));
  pen.setWidth(2);
  path->setPen(pen);
  QPainterPath ppath;
  ppath.moveTo(-150, -200);
  ppath.cubicTo(0, -200, 0, 200, 150, 200);
  path->setPath(ppath);
  m_scene->addItem(path);
  */


  {
    QGraphicsItemGroup *group = new QGraphicsItemGroup();

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    rect->setFlag(QGraphicsItem::ItemIsMovable, true);
    rect->setRect(0, 0, 200, 100);
    rect->setBrush(QBrush(QColor(196, 196, 196)));
    rect->setPen(QPen(QColor(64, 64, 64)));

    QGraphicsRectItem *rect2 = new QGraphicsRectItem(rect);
    rect2->setParentItem(rect);
    rect2->setRect(20, 20, 160, 20);
    rect2->setBrush(QBrush(QColor(255, 255, 255)));
    rect2->setPen(QPen(QColor(64, 64, 64)));

    QGraphicsTextItem *text = new QGraphicsTextItem(rect2);
    text->setParentItem(rect2);
    text->setPos(20, 20);
    text->setPlainText("Hello World!");
    text->setTextInteractionFlags(Qt::TextEditorInteraction);
    text->setTextWidth(160);

    group->setHandlesChildEvents(false);
    group->addToGroup(rect);
    group->addToGroup(rect2);
    group->addToGroup(text);

    //    m_scene->addItem(group);
  }


  addNode(vkSGFloat4::GetStaticClass());



}

ShaderGraphView::~ShaderGraphView()
{

}


void  ShaderGraphView::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    popupNodeSelector ();
  }
}

void ShaderGraphView::popupNodeSelector()
{
  NodeSelector *selector = new NodeSelector(this);
  connect(selector, SIGNAL(addNode(const vkClass*)), this, SLOT(addNode(const vkClass*)));
  selector->setVisible(true);
}

void ShaderGraphView::moveDrag(bool input, int idx, const QPointF& pointA, const QPointF & pointB)
{
  printf ("Move %d %d (%f %f) => (%f %f)\n", input, idx, pointA.x(), pointA.y(), pointB.x(), pointB.y());
  fflush(stdout);
  if (!m_currentBounding)
  {
    m_currentBounding = new QGraphicsPathItem();
    m_currentBounding->setPen(QPen(QBrush(QColor(255, 255, 255)), 2.0f));
    m_scene->addItem(m_currentBounding);
  }


  ShaderGraphNode *node = (ShaderGraphNode*)sender();

  QPointF pB = testAnchor(node, input, pointB);

  float cX = (pointA.x() + pB.x()) / 2.0f;

  QPainterPath ppath;
  ppath.moveTo(pointA);
  ppath.cubicTo(QPointF(cX, pointA.y()), QPointF(cX, pB.y()), pB);
  m_currentBounding->setPath(ppath);
}

void ShaderGraphView::stopDrag(bool input, int idx, const QPointF& pointA, const QPointF & pointB)
{
  if (m_currentBounding)
  {
    m_scene->removeItem(m_currentBounding);
    delete m_currentBounding;
    m_currentBounding = 0;

    ShaderGraphNode *node = (ShaderGraphNode*)sender();
    ShaderGraphNode *other;
    int index;
    if (testAnchor(node, input, pointB, &other, &index))
    {
      ShaderGraphConnection *connection;
      if (input)
      {
        connection = new ShaderGraphConnection(other, index, node, idx);
        node->ConnectInput(connection, idx);
        other->ConnectOutput(connection, index);
      }
      else
      {
        connection = new ShaderGraphConnection(node, idx, other, index);
        node->ConnectOutput(connection, idx);
        other->ConnectInput(connection, index);
      }

      m_scene->addItem(connection->GetItem());
    }

  }
}

QPointF ShaderGraphView::testAnchor(ShaderGraphNode *node, bool input, const QPointF &p)
{
  for (size_t i=0, in=m_nodes.size(); i<in; ++i)
  {
    ShaderGraphNode *sgn = m_nodes[i];
    if (sgn != node)
    {
      QPointF r;
      int index;
      if (sgn->hasAnchor(input, p, r, index))
      {
        return r;
      }
    }
  }
  return p;
}

bool ShaderGraphView::testAnchor (ShaderGraphNode *node, bool input, const QPointF &p, ShaderGraphNode **other, int *index)
{
  for (size_t i=0, in=m_nodes.size(); i<in; ++i)
  {
    ShaderGraphNode *sgn = m_nodes[i];
    if (sgn != node)
    {
      QPointF r;
      int idx;
      if (sgn->hasAnchor(input, p, r, idx))
      {
        *other = sgn;
        *index = idx;
        return true;
      }
    }
  }
  return false;
}


void ShaderGraphView::addNode(const vkClass *clazz)
{
  vkSGNode *node = clazz->CreateInstance<vkSGNode>();
  if (!node)
  {
    return;
  }

  ShaderGraphNode *graphNode = new ShaderGraphNode(node);
  m_scene->addItem(graphNode->GetItem());

  connect (graphNode, SIGNAL(moveDrag(bool, int, const QPointF&, const QPointF &)), this, SLOT(moveDrag(bool, int, const QPointF&, const QPointF &)));
  connect (graphNode, SIGNAL(stopDrag(bool, int, const QPointF&, const QPointF &)), this, SLOT(stopDrag(bool, int, const QPointF&, const QPointF &)));

  m_nodes.append(graphNode);
}

