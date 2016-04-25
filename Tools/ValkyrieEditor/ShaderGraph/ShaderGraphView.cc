
#include <ShaderGraph/ShaderGraphView.hh>
#include <ShaderGraph/NodeSelector.hh>
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <Graph/Connection.hh>
#include <Graph/Node.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>
#include <qevent.h>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>


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


  m_scene = new graph::NodeGraphScene();

  connect(m_scene, SIGNAL(NodeConnectedLooseInput(graph::Node *, int)), this, SLOT(NodeConnectedLooseInput(graph::Node *, int)));
  m_view->setScene(m_scene);

  m_shaderGraph = new vkSGShaderGraph();
  shadergraph::SGShaderGraphNode *shaderGraphNode = new shadergraph::SGShaderGraphNode(m_shaderGraph);
  shaderGraphNode->Initialize();
  m_scene->AddNode(shaderGraphNode);

  AddNode(vkSGConstFloat2::GetStaticClass());
  AddNode(vkSGConstFloat2::GetStaticClass());
  AddNode(vkSGAdd::GetStaticClass());

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
  else if (event->key() == Qt::Key_Delete)
  {
    graph::Node *selectedNode = graph::Node::GetSelected();
    if (!selectedNode)
    {
      return;
    }
    if (static_cast<shadergraph::SGNode*>(selectedNode)->GetType () == shadergraph::SGNode::eT_Node)
    {
      m_scene->RemoveSelectedNode();
    }
  }
}

void ShaderGraphView::popupNodeSelector()
{
  NodeSelector *selector = new NodeSelector(this);
  connect(selector, SIGNAL(addNode(const vkClass*)), this, SLOT(AddNode(const vkClass*)));
  selector->setVisible(true);
}


graph::Node *ShaderGraphView::AddNode(const vkClass *clazz)
{
  vkSGNode *node = clazz->CreateInstance<vkSGNode>();
  if (!node)
  {
    return 0;
  }

  shadergraph::SGNode *sgNode = new shadergraph::SGNode(node);
  if (!sgNode->Initialize())
  {
    delete sgNode;
    node->Release();
    return 0;
  }

  m_scene->AddNode(sgNode);
  return sgNode;
}

void ShaderGraphView::NodeConnectedLooseInput(graph::Node *inputNode, int inputIdx)
{
  m_scene->DisconnectInput(inputNode, inputIdx);
}
