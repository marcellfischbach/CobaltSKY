
#include <ShaderGraph/ShaderGraphView.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>

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
  m_view = new QGraphicsView(this);
  m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_view);


  m_view->setBackgroundBrush(QBrush(QColor(32, 32, 32)));


  m_scene = new QGraphicsScene();
  m_view->setScene(m_scene);

  GraphNode *graphNode = new GraphNode(0);
  graphNode->setPos(-200, -200);
  m_scene->addItem(graphNode);


  graphNode = new GraphNode(0);
  graphNode->setPos(200, 200);
  m_scene->addItem(graphNode);

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