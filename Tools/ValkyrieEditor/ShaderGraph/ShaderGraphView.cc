
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

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>


ShaderGraphView::ShaderGraphView(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);



  setMouseTracking(true);
  m_view = new QGraphicsView(this);
  m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

  QGridLayout *layout = new QGridLayout(m_gui.wShaderGraphView);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_view);


  m_view->setBackgroundBrush(QBrush(QColor(32, 32, 32)));
  m_scene = new graph::NodeGraphScene();
  connect(m_scene, SIGNAL(NodeConnectedLooseInput(graph::Node *, int)), this, SLOT(NodeConnectedLooseInput(graph::Node *, int)));
  m_view->setScene(m_scene);

  
  m_gui.cbDiscardAlphaCompareMode->addItem("<= (less or equal)", QVariant(eCM_LessOrEqual));
  m_gui.cbDiscardAlphaCompareMode->addItem(">= (greater or equal)", QVariant(eCM_GreaterOrEqual));
  m_gui.cbDiscardAlphaCompareMode->addItem("< (less)", QVariant(eCM_Less));
  m_gui.cbDiscardAlphaCompareMode->addItem("> (greater)", QVariant(eCM_Greater));
  m_gui.cbDiscardAlphaCompareMode->addItem("== (equal)", QVariant(eCM_Equal));
  m_gui.cbDiscardAlphaCompareMode->addItem("!= (not equal)", QVariant(eCM_NotEqual));
  on_cbDiscardAlpha_stateChanged(0);


  Setup(new vkSGShaderGraph());
}

ShaderGraphView::~ShaderGraphView()
{

}

void ShaderGraphView::Setup(vkSGShaderGraph *shaderGraph)
{
  m_shaderGraph = shaderGraph;
  shadergraph::SGShaderGraphNode *shaderGraphNode = new shadergraph::SGShaderGraphNode(m_shaderGraph);
  shaderGraphNode->Initialize();
  m_scene->AddNode(shaderGraphNode);

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

void ShaderGraphView::on_cbDiscardAlpha_stateChanged(int state)
{
  m_gui.cbDiscardAlphaCompareMode->setEnabled(state != 0);
  m_gui.sbDiscardAlphaThreshold->setEnabled(state != 0);
}


void ShaderGraphView::on_pbCompile_clicked(bool)
{
  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraph))
  {
    printf("Unable to compile\n");
  }
}

void ShaderGraphView::on_pbSave_clicked(bool)
{
  QVector<shadergraph::SGNode*> nodes;
  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {
    graph::Node *grNode = m_scene->GetNode(i);
    shadergraph::Node *node = static_cast<shadergraph::Node*>(grNode);
    if (node->GetType() == shadergraph::Node::eT_Node)
    {
      nodes.append(static_cast<shadergraph::SGNode*>(node));
    }
  }


  QVector<graph::NodeConnection*> connections;
  for (size_t i = 0, in = m_scene->GetNumberOfConnections(); i < in; ++i)
  {
    graph::NodeConnection *connection = m_scene->GetConnection(i);

    shadergraph::Node *node = static_cast<shadergraph::Node*>(connection->GetInputNode());
    if (node->GetType() == shadergraph::Node::eT_Node)
    {
      shadergraph::SGNode *inputNode = static_cast<shadergraph::SGNode*>(connection->GetInputNode());
      shadergraph::SGNode *outputNode = static_cast<shadergraph::SGNode*>(connection->GetOutputNode());
      if (nodes.contains(inputNode) && (!outputNode || nodes.contains(outputNode)))
      {
        connections.append(connection);
      }
    }
  }



  vkAssetOutputStream os;
  os << (vkUInt16)nodes.size();
  for (size_t i = 0, in = nodes.size(); i < in; ++i)
  {
    shadergraph::SGNode *node = nodes[i];
    vkSGNode *sgNode = node->GetNode();
    os << (vkUInt32)i << vkString(sgNode->GetClass()->GetName());
  }

  os << (vkUInt16)connections.size();
  for (size_t i = 0, in = connections.size(); i < in; ++i)
  {
    graph::NodeConnection *connection = connections[i];
    shadergraph::SGNode *inputNode = static_cast<shadergraph::SGNode*>(connection->GetInputNode());
    shadergraph::SGNode *outputNode = static_cast<shadergraph::SGNode*>(connection->GetOutputNode());

    vkSGInput *input = inputNode->GetNode()->GetInput(connection->GetInputIdx());
    vkUInt8 inputType = outputNode ? 1 : 0;
    os << (vkUInt32)nodes.indexOf(inputNode)
      << (vkUInt8)connection->GetInputIdx()
      << inputType
      << (float)(input ? input->GetConst() : 0.0f)
      << (vkUInt32)nodes.indexOf(outputNode)
      << (vkUInt8)connection->GetOutputIdx();
  }

  struct Attrib
  {
    vkUInt8 attrib;
    vkUInt32 nodeIdx;
    vkUInt8 nodeOutputIdx;
  };

  QVector<Attrib> attribs;
  if (m_shaderGraph->GetDiffuse())
  {

  }


}
