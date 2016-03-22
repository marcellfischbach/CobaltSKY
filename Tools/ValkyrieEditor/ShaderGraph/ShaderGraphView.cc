
#include <ShaderGraph/Node.hh>
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



  ShaderGraphNode *node = new ShaderGraphNode (new vkSGFloat4());
  m_scene->addItem(node->GetItem());


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

void ShaderGraphView::startConnectingInput(const QPointF &pos, int i)
{

}

void ShaderGraphView::startConnectingOutput(const QPointF &pos, int i)
{

}

void ShaderGraphView::processConnecion(const QPointF &pos)
{

}

void ShaderGraphView::addNode(const vkClass *clazz)
{
  vkSGNode *node = clazz->CreateInstance<vkSGNode>();
  if (!node)
  {
    return;
  }

  ShaderGraphNode2 *graphNode = new ShaderGraphNode2();
  graphNode->SetNode(node);
  graphNode->Initialize();
  m_scene->addItem(graphNode);

  //  connect(graphNode, SIGNAL(startConnectionInput(QPointF,int)), this, SLOT(startConnectingInput(QPointF,int)));
  //  connect(graphNode, SIGNAL(startConnectionOutput(QPointF,int)), this, SLOT(startConnectingOutput(QPointF,int)));
  //  connect(graphNode, SIGNAL(processConnection(QPointF)), this, SLOT(processConnecion(QPointF)));
}

