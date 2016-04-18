
#include <ShaderGraph/ShaderGraphView.hh>
#include <ShaderGraph/NodeSelector.hh>
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

  connect(m_scene, SIGNAL(NodeAdded(graph::Node *)), this, SLOT(NodeAdded(graph::Node *)));
  connect(m_scene, SIGNAL(NodeRemoved(graph::Node *)), this, SLOT(NodeRemoved(graph::Node *)));
  connect(m_scene, SIGNAL(NodesConnected(graph::Node *, int, graph::Node *, int)), this, SLOT(NodesConnected(graph::Node *, int, graph::Node *, int)));
  connect(m_scene, SIGNAL(NodesDisconnected(graph::Node *, int, graph::Node *, int)), this, SLOT(NodesDisconnected(graph::Node *, int, graph::Node *, int)));
  connect(m_scene, SIGNAL(NodeConnectedLooseInput(graph::Node *, int)), this, SLOT(NodeConnectedLooseInput(graph::Node *, int)));
  m_view->setScene(m_scene);


  AddNode(vkSGConstFloat2::GetStaticClass());
  AddNode(vkSGConstFloat2::GetStaticClass());
  AddNode(vkSGAdd::GetStaticClass());

  /*
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

  */
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
    m_scene->RemoveSelectedNode();
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

  if (!graphNode->Initialize())
  {
    delete graphNode;
    graphNode = 0;
    return 0;
  }
  m_nodes[graphNode] = node;
  m_nodesRev[node] = graphNode;
  node->AddRef();

  m_scene->AddNode(graphNode);
  return graphNode;
}

void ShaderGraphView::NodeAdded(graph::Node *node)
{
  // nodes can only be added via the AddNode(const vkClass*) method...
  // but than it is already within the map
  return;
}

void ShaderGraphView::NodeRemoved(graph::Node *node)
{
  if (!m_nodes.contains(node))
  {
    return;
  }

  vkSGNode *vkNode = m_nodes[node];
  RemoveAllConnections(vkNode);
  m_nodes.remove(node);
  m_nodesRev.remove(vkNode);

  vkNode->Release();
}

void ShaderGraphView::NodesConnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx)
{
  if (!m_nodes.contains(outNode) || !m_nodes.contains(inNode))
  {
    return;
  }
  vkSGNode *inputNode = m_nodes[inNode];
  vkSGNode *outputNode = m_nodes[outNode];


  // check if there is already a connection to this input
  vkSGInput *input = inputNode->GetInput(inIdx);
  vkSGOutput *output = input->GetInput();
  if (output)
  {
    vkSGNode *outputNode = output->GetNode();
    m_scene->Disconnect(m_nodesRev[outputNode], output->GetIdx(), inNode, inIdx);

  }

  output = outputNode->GetOutput(outIdx);
  input->SetInput(output);
}

void ShaderGraphView::NodesDisconnected(graph::Node *outNode, int outIdx, graph::Node *inNode, int inIdx)
{
  if (!m_nodes.contains(outNode) || !m_nodes.contains(inNode))
  {
    return;
  }
  vkSGNode *inputNode = m_nodes[inNode];
  vkSGNode *outputNode = m_nodes[outNode];

  vkSGOutput *output = outputNode->GetOutput(outIdx);
  vkSGInput *input = inputNode->GetInput(inIdx);

  if (input->GetInput() == output)
  {
    input->SetInput(0);
  }

}

void ShaderGraphView::NodeConnectedLooseInput(graph::Node *inputNode, int inputIdx)
{
  m_scene->DisconnectInput(inputNode, inputIdx);
}

void ShaderGraphView::RemoveAllConnections(vkSGNode *node)
{
  if (!node)
  {
    return;
  }

  for (vkSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = node->GetInput(i);
    vkSGOutput *output = input->GetInput();
    if (output)
    {
      int outputIdx = output->GetIdx();
      m_scene->Disconnect(m_nodesRev[output->GetNode()], outputIdx, m_nodesRev[node], i);
      input->SetInput(0);
    }
  }

  QList<vkSGNode*> &nodes = m_nodesRev.keys();
  for (vkSGNode *n : nodes)
  {
    for (vkSize i = 0, in = n->GetNumberOfInputs(); i < in; ++i)
    {
      vkSGInput *input = n->GetInput(i);
      vkSGOutput *output = input->GetInput();
      if (output)
      {
        vkSGNode *outputNode = output->GetNode();
        int outputIdx = output->GetIdx();
        if (outputNode == node)
        {
          m_scene->Disconnect(m_nodesRev[node], outputIdx, m_nodesRev[n], i);
          input->SetInput(0);
        }
      }
    }
  }
}