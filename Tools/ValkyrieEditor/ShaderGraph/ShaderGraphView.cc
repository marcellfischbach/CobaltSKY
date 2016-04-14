
#include <ShaderGraph/Node.hh>
#include <ShaderGraph/Connection.hh>
#include <ShaderGraph/NodeSelector.hh>
#include <ShaderGraph/ShaderGraphView.hh>
#include <Graph/Node.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>
#include <qevent.h>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>

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
  m_view->setScene(m_scene);


  graph::Node *node = new graph::Node();
  node->SetLabel("Hello World 1");
  node->AddInput("x", "x", graph::Node::eIM_Both);
  node->AddInput("y", "y", graph::Node::eIM_Both);
  node->AddInput("z", "z", graph::Node::eIM_Both);
  node->AddOutput("v", "v");
  node->AddOutput("v1", "v1");
  node->AddOutput("v2", "v2");
  node->AddOutput("v3", "v3");

  if (node->Initialize())
  {
    m_scene->AddNode(node);
    node->SetPosition(QPointF(-100, 0));
  }


  node = new graph::Node();
  node->SetLabel("Hello World 2");
  node->AddInput("x", "x", graph::Node::eIM_Both);
  node->AddInput("y", "y", graph::Node::eIM_Both);
  node->AddInput("z", "z", graph::Node::eIM_Both);
  node->AddOutput("v", "v");
  node->AddOutput("v1", "v1");
  node->AddOutput("v2", "v2");
  node->AddOutput("v3", "v3");

  if (node->Initialize())
  {
    m_scene->AddNode(node);
    node->SetPosition(QPointF(100, 0));
  }


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
  connect(selector, SIGNAL(addNode(const vkClass*)), this, SLOT(AddNode(const vkClass*)));
  selector->setVisible(true);
}


void ShaderGraphView::AddNode(const vkClass *clazz)
{
  vkSGNode *node = clazz->CreateInstance<vkSGNode>();
  if (!node)
  {
    return;
  }

  graph::Node *graphNode = new graph::Node();
  graphNode->SetLabel(QString(node->GetName().c_str()));
  
  for (vkSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = node->GetInput(i);
    QString inputName(input->GetName().c_str());

    graph::Node::InputMode mode;
    if (input->CanInputConst ())
    {
      mode = (graph::Node::InputMode)(mode | graph::Node::eIM_Const);
    }
    if (input->CanInputNode())
    {
      mode = (graph::Node::InputMode)(mode | graph::Node::eIM_Output);
    }
    graphNode->AddInput(inputName, inputName, mode);
  }
  for (vkSize i = 0, in = node->GetNumberOfOutputs(); i < in; ++i)
  {
    vkSGOutput *output = node->GetOutput(i);
    QString outputName(output->GetName().c_str());

    graphNode->AddOutput(outputName, outputName);
  }

  m_scene->AddNode(graphNode);

  m_nodes[graphNode] = node;
}

void ShaderGraphView::NodeAdded(graph::Node *node)
{

}

void ShaderGraphView::NodeRemoved(graph::Node *node)
{

}

void ShaderGraphView::NodesConnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx)
{

}

void ShaderGraphView::NodesDisnnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx)
{

}
