
#include <ShaderGraph/ShaderGraphView.hh>
#include <ShaderGraph/NodeSelector.hh>
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <AssetManager/AssetWriter.hh>
#include <Graph/Connection.hh>
#include <Graph/Node.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>
#include <qevent.h>

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>


ShaderGraphView::ShaderGraphView(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);

  m_resourceLocator = vkResourceLocator("${materials}/my_material.asset");

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
  m_shaderGraphNode = new shadergraph::SGShaderGraphNode(m_shaderGraph);
  m_shaderGraphNode->Initialize();
  m_scene->AddNode(m_shaderGraphNode);

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
  CollectData();
  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraph))
  {
    printf("Unable to compile\n");
  }
}

void ShaderGraphView::on_pbSave_clicked(bool)
{
  if (m_resourceLocator.GetResourceFile().length() == 0)
  {
    return;
  }


  CollectData();
  QVector<shadergraph::SGNode*> nodes;
  QVector<vkSGNode*> sgNodes;
  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {
    graph::Node *grNode = m_scene->GetNode(i);
    shadergraph::Node *node = static_cast<shadergraph::Node*>(grNode);
    if (node->GetType() == shadergraph::Node::eT_Node)
    {
      shadergraph::SGNode *sgNode = static_cast<shadergraph::SGNode*>(node);
      nodes.append(sgNode);
      sgNodes.append(sgNode->GetNode());
    }
  }


  QVector<vkSGInput*> inputs;

  for (size_t i = 0, in = nodes.size(); i < in; ++i)
  {
    vkSGNode* node = sgNodes[i];
    for (size_t j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    { 
      vkSGInput* input = node->GetInput(j);
      inputs.push_back(input);
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

  os << (vkUInt16)inputs.size();
  for (size_t i = 0, in = inputs.size(); i < in; ++i)
  {
    vkSGInput *input = inputs[i];
    vkSGNode *inputNode = input->GetNode();
    vkUInt32 inputIdx = input->GetIdx();

    vkSGOutput *output = input->GetInput();
    vkSGNode *outputNode = 0;
    vkUInt32 outputIdx = 0;
    if (output)
    {
      outputIdx = output->GetIdx();
      outputNode = output->GetNode();
    }

    vkUInt8 inputType = output ? 1 : 0;
    os << (vkUInt32)sgNodes.indexOf(inputNode)
      << (vkUInt8)inputIdx
      << inputType
      << (float)input->GetConst()
      << (vkUInt32)sgNodes.indexOf(outputNode)
      << (vkUInt8)outputIdx;
  }

  struct Attrib
  {
    vkUInt8 attrib;
    vkUInt32 nodeIdx;
    vkUInt8 nodeOutputIdx;
  };

  QVector<Attrib> attribs;
  for (unsigned i = 0; i < vkSGShaderGraph::eIT_COUNT; ++i)
  {
    vkSGOutput *output = m_shaderGraph->GetInput((vkSGShaderGraph::InputType)i);
    if (output)
    {
      vkSGNode *outputNode = output->GetNode();
      Attrib attr;
      attr.attrib = i;
      attr.nodeIdx = sgNodes.indexOf(outputNode);
      attr.nodeOutputIdx = output->GetIdx();
      attribs.append(attr);
    }
  }

  os << (vkUInt16)attribs.size();
  for (Attrib &attr : attribs)
  {
    os << attr.attrib
      << attr.nodeIdx
      << attr.nodeOutputIdx;
  }


 
  os << (vkUInt16)2
    // binary gradient
    << vkString ("blendOutWithBinaryGradient")
    << (vkUInt8)(m_shaderGraph->IsBlendOutWithBinaryGradient() ? 1 : 0)
    // discard alpha
    << vkString("discardAlpha")
    << (vkUInt8)(m_shaderGraph->IsDiscardAlpha() ? 1 : 0)
    << (vkUInt8)(m_shaderGraph->GetDiscardAlphaCompareMode())
    << (float)m_shaderGraph->GetDiscardAlphaThreshold();


  IFile *file = vkVFS::Get()->Open(m_resourceLocator.GetResourceFile(), eOM_Write, eTM_Binary);
  AssetWriter writer;
  writer.AddEntry("SHADER_GRAPH", "DATA", os.GetSize(), os.GetBuffer());
  
  writer.Output(file);
  file->Close();
}




void ShaderGraphView::CollectData()
{
  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {
    shadergraph::Node* node = static_cast<shadergraph::Node*>(m_scene->GetNode(i));
    if (node->GetType() != shadergraph::Node::eT_Node)
    {
      continue;
    }

    shadergraph::SGNode* sgNode = static_cast<shadergraph::SGNode*>(node);

    for (size_t j = 0, jn = sgNode->GetNumberOfInputs(); j < jn; ++j)
    {
      float constFloat = sgNode->GetConstInput(j);
      sgNode->GetNode()->GetInput(j)->SetConst(constFloat);
    }

    vkSGNode *vksgNode = sgNode->GetNode();
    vkSGResourceNode *resNode = vkQueryClass<vkSGResourceNode>(vksgNode);
    if (resNode && sgNode->HasName())
    {
      resNode->SetResourceName(vkString((const char*)sgNode->GetName().toLatin1()));
    }
  }


  m_shaderGraph->SetBlendOutWithBinaryGradient(m_gui.cbBlendBinaryGradient->checkState() != 0);
  m_shaderGraph->SetDiscardAlpha(m_gui.cbDiscardAlpha->checkState() != 0);
  m_shaderGraph->SetDiscardAlpha((float)m_gui.sbDiscardAlphaThreshold->value(),
                                 (vkCompareMode)m_gui.cbDiscardAlphaCompareMode->currentIndex());
}