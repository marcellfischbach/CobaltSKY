
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

  ShaderGraphNode *node = new ShaderGraphNode();
  node->SetNode(new vkSGFloat4());
  node->Initialize();
  m_scene->addItem(node);



  QGraphicsPathItem *path = new QGraphicsPathItem();
  QPen pen(QColor(255, 255, 255));
  pen.setWidth(2);
  path->setPen(pen);
  QPainterPath ppath;
  ppath.moveTo(-150, -200);
  ppath.cubicTo(0, -200, 0, 200, 150, 200);
  path->setPath(ppath);
  m_scene->addItem(path);

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

void ShaderGraphView::addNode(const vkClass *clazz)
{
  vkSGNode *node = clazz->CreateInstance<vkSGNode>();
  if (!node)
  {
    return;
  }

  ShaderGraphNode *graphNode = new ShaderGraphNode();
  graphNode->SetNode(node);
  graphNode->Initialize();
  m_scene->addItem(graphNode);
}

