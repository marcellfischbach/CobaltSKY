
#include <ShaderGraph/ShaderGraphView.hh>
#include <ShaderGraph/EditResourceDefaultDialog.hh>
#include <ShaderGraph/MetaData.hh>
#include <ShaderGraph/NodeSelector.hh>
#include <ShaderGraph/PreviewWidget.hh>
#include <ShaderGraph/ResourcesModel.hh>
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <AssetManager/AssetWriter.hh>
#include <Graph/Connection.hh>
#include <Graph/Node.hh>
#include <Editor.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qpainterpath.h>
#include <qevent.h>
#include <qtimer.h>

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>


ShaderGraphView::ShaderGraphView(QWidget *parent)
  : QWidget(parent)
  , m_resourcesModel(0)
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
  connect(m_scene, SIGNAL(NodeNameChanged(graph::Node*)), this, SLOT(NodeNameChanged(graph::Node*)));
  connect(m_scene, SIGNAL(NodeRemoved(graph::Node*)), this, SLOT(NodeRemoved(graph::Node*)));
  m_view->setScene(m_scene);


  m_gui.cbDiscardAlphaCompareMode->addItem("<= (less or equal)", QVariant(eCM_LessOrEqual));
  m_gui.cbDiscardAlphaCompareMode->addItem(">= (greater or equal)", QVariant(eCM_GreaterOrEqual));
  m_gui.cbDiscardAlphaCompareMode->addItem("< (less)", QVariant(eCM_Less));
  m_gui.cbDiscardAlphaCompareMode->addItem("> (greater)", QVariant(eCM_Greater));
  m_gui.cbDiscardAlphaCompareMode->addItem("== (equal)", QVariant(eCM_Equal));
  m_gui.cbDiscardAlphaCompareMode->addItem("!= (not equal)", QVariant(eCM_NotEqual));
  on_cbDiscardAlpha_stateChanged(0);

  m_resourcesModel = new shadergraph::ResourcesModel();
  m_gui.tvResources->setModel(m_resourcesModel);

  connect(m_gui.tvResources, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(ResourceDoubleClicked(const QModelIndex&)));

  m_previewWidget = new shadergraph::PreviewWidget(this);
  layout = new QGridLayout(m_gui.wPreview);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_previewWidget);

}


ShaderGraphView::~ShaderGraphView()
{

}
void ShaderGraphView::NodeRemoved(graph::Node *node)
{
  shadergraph::Node* gNode = static_cast<shadergraph::Node*>(node);
  if (gNode->GetType() == shadergraph::Node::eT_Node)
  {
    m_resourcesModel->RemoveNode(static_cast<shadergraph::SGNode*>(node));
  }
}

void ShaderGraphView::NodeNameChanged(graph::Node *node)
{
  shadergraph::Node *gNode = static_cast<shadergraph::Node*>(node);
  if (gNode->GetType() == shadergraph::Node::eT_Node && m_resourcesModel)
  {
    m_resourcesModel->NodeChanged(static_cast<shadergraph::SGNode*>(node));
  }
}

void ShaderGraphView::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;

  vkResourceLocator metaLocator = vkResourceLocator(m_resourceLocator.GetResourceFile(), "META_DATA");

  vkSGShaderGraph *shaderGraph = vkResourceManager::Get()->GetOrLoad<vkSGShaderGraph>(m_resourceLocator);
  ShaderGraphMetaData *metaData = vkResourceManager::Get()->GetOrLoad<ShaderGraphMetaData>(metaLocator);
  bool nullShader = !shaderGraph;
  if (nullShader)
  {
    shaderGraph = new vkSGShaderGraph();
  }
  Setup(shaderGraph, metaData);

  if (nullShader)
  {
    Compile();
  }
}

void ShaderGraphView::Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData)
{
  m_shaderGraph = shaderGraph;
  m_shaderGraphNode = new shadergraph::SGShaderGraphNode(shaderGraph);
  m_shaderGraphNode->Initialize();
  m_scene->AddNode(m_shaderGraphNode);

  if (metaData)
  {
    vkVector2f graphPos = metaData->GetGraphPosition();
    m_shaderGraphNode->SetPosition(QPointF(graphPos.x, graphPos.y));
  }

  std::map<vkSGNode*, graph::Node*> nodes;
  for (size_t i = 0, in = shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *sgNode = shaderGraph->GetNode(i);
    vkVector2f nodePosition = metaData ? metaData->GetNodePosition(i) : vkVector2f(0.0f, 0.0f);
    graph::Node *gnode = AddNode(sgNode, nodePosition);
    nodes[sgNode] = gnode;
  }


  for (size_t i = 0, in = shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *inputNode = shaderGraph->GetNode(i);
    if (nodes.find(inputNode) == nodes.end())
    {
      continue;
    }
    graph::Node *gInputNode = nodes[inputNode];


    for (size_t j = 0, jn = inputNode->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput *input = inputNode->GetInput(j);
      if (input->CanInputNode())
      {
        vkSGOutput *output = input->GetInput();
        if (output)
        {
          vkSGNode *outputNode = output->GetNode();
          if (outputNode)
          {
            if (nodes.find(outputNode) != nodes.end())
            {
              graph::Node *gOutputNode = nodes[outputNode];
              m_scene->Connect(gOutputNode, output->GetIdx(), gInputNode, input->GetIdx());
            }
          }
        }
      }
      if (input->CanInputConst())
      {
        gInputNode->SetConstInput(j, input->GetConst());
      }
    }
  }

  for (size_t i = 0, in = vkSGShaderGraph::eIT_COUNT; i < in; ++i)
  {
    vkSGOutput *output = shaderGraph->GetInput((vkSGShaderGraph::InputType)i);
    if (output)
    {
      vkSGNode *outputNode = output->GetNode();
      if (nodes.find(outputNode) != nodes.end())
      {
        graph::Node *gOutputNode = nodes[outputNode];
        m_scene->Connect(gOutputNode, output->GetIdx(), m_shaderGraphNode, i);
      }
    }
  }
}


void  ShaderGraphView::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    popupNodeSelector();
  }
  else if (event->key() == Qt::Key_Delete)
  {
    graph::Node *selectedNode = graph::Node::GetSelected();
    if (!selectedNode)
    {
      return;
    }
    if (static_cast<shadergraph::SGNode*>(selectedNode)->GetType() == shadergraph::SGNode::eT_Node)
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
  return AddNode(clazz, 0, vkVector2f(0.0f, 0.0f));
}

graph::Node *ShaderGraphView::AddNode(const vkSGNode *node, const vkVector2f &pos)
{
  if (!node)
  {
    return 0;
  }
  return AddNode(node->GetClass(), node, pos);

}

graph::Node *ShaderGraphView::AddNode(const vkClass *clazz, const vkSGNode *node, const vkVector2f &pos)
{
  if (!clazz)
  {
    return 0;
  }
  shadergraph::SGNode *sgNode = new shadergraph::SGNode(clazz, node);
  if (!sgNode->Initialize())
  {
    delete sgNode;
    return 0;
  }

  if (sgNode->IsResources())
  {
    m_resourcesModel->AddNode(sgNode);
  }
  m_scene->AddNode(sgNode);
  sgNode->SetPosition(QPointF(pos.x, pos.y));
  return sgNode;
}




void ShaderGraphView::NodeConnectedLooseInput(graph::Node *inputNode, int inputIdx)
{
  m_scene->DisconnectInput(inputNode, inputIdx);
}

void ShaderGraphView::ResourceDoubleClicked(const QModelIndex &index)
{
  if (!index.isValid())
  {
    return;
  }

  shadergraph::SGNode* node = static_cast<shadergraph::SGNode*>(index.internalPointer());
  if (!node)
  {
    return;
  }

  shadergraph::EditResourceDefaultDialog *dialog = new shadergraph::EditResourceDefaultDialog(this);
  connect(dialog, SIGNAL(Changed(shadergraph::SGNode*)), this, SLOT(ResourceEditApplied(shadergraph::SGNode*)));
  dialog->SetNode(node);
  dialog->setVisible(true);
}

void ShaderGraphView::ResourceEditApplied(shadergraph::SGNode* node)
{
  m_resourcesModel->NodeChanged(node);
  connect(sender(), SIGNAL(Changed(shadergraph::SGNode*)), this, SLOT(ResourceEditApplied(shadergraph::SGNode*)));
}

void ShaderGraphView::on_cbDiscardAlpha_stateChanged(int state)
{
  m_gui.cbDiscardAlphaCompareMode->setEnabled(state != 0);
  m_gui.sbDiscardAlphaThreshold->setEnabled(state != 0);
}

bool ShaderGraphView::Compile()
{
  vkSGShaderGraph compileGraph;
  std::map<graph::Node*, vkSGNode*> nodes;
  CollectData(&compileGraph, nodes);
  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(&compileGraph))
  {
    for (auto it : nodes)
    {
      printf("Node: %s\n", it.second->GetValidationMessage().c_str());
    }
    return false;
  }

  vkSGShaderGraph *shaderGraph = vkResourceManager::Get()->GetOrLoad<vkSGShaderGraph>(m_resourceLocator);
  if (!shaderGraph)
  {
    return false;
  }
  CollectData(shaderGraph, nodes);

  m_previewWidget->SetMaterial(shaderGraph);
  m_previewWidget->repaint();
  return true;
}

void ShaderGraphView::on_pbCompile_clicked(bool)
{
  Compile();
}
  

namespace
{
void invert(const std::map<graph::Node*, vkSGNode*> &in, std::map<vkSGNode*, graph::Node*> &out)
{
  for (auto v : in)
  {
    out[v.second] = v.first;
  }
}
}

void ShaderGraphView::on_pbSave_clicked(bool)
{
  if (m_resourceLocator.GetResourceFile().length() == 0)
  {
    return;
  }

  Compile();

  vkSGShaderGraph tempGraph;
  std::map<graph::Node*, vkSGNode*> nodes;
  CollectData(&tempGraph, nodes);
  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(&tempGraph))
  {
    return;
  }
  std::map<vkSGNode*, graph::Node*> nodesInv;
  ::invert(nodes, nodesInv);

  QVector<vkSGNode*> sgNodes;
  for (size_t i = 0, in = tempGraph.GetNumberOfTotalNodes(); i < in; ++i)
  {
    sgNodes.append(tempGraph.GetNode(i));
  }


  QVector<vkSGInput*> inputs;

  for (size_t i = 0, in = sgNodes.size(); i < in; ++i)
  {
    vkSGNode* node = sgNodes[i];
    for (size_t j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput* input = node->GetInput(j);
      inputs.push_back(input);
    }
  }


  QPointF shaderGraphPos = m_shaderGraphNode->GetItem()->pos();
  vkAssetOutputStream osMeta;
  osMeta << (vkUInt32)VK_VERSION(1, 0, 0);
  osMeta << vkVector2f(shaderGraphPos.x(), shaderGraphPos.y());

  vkAssetOutputStream osData;
  osData << (vkUInt32)VK_VERSION(1, 0, 0);
  osData << (vkUInt16)nodes.size();
  osMeta << (vkUInt16)nodes.size();
  for (size_t i = 0, in = sgNodes.size(); i < in; ++i)
  {
    vkSGNode *sgNode = sgNodes[i];
    osData << (vkUInt32)i << vkString(sgNode->GetClass()->GetName());

    shadergraph::SGNode *graphNode = static_cast<shadergraph::SGNode*>(nodesInv[sgNode]);
    if (graphNode->IsResources())
    {
      float *floats = graphNode->GetDefaultFloat();
      int *ints = graphNode->GetDefaultInt();
      vkResourceLocator &txtResLocator = graphNode->GetDefaultTexture();
      osData << vkString((const char*)graphNode->GetName().toLatin1());
      switch (graphNode->GetResourceType())
      {
      case eSPT_Float:
        osData << floats[0];
        break;
      case eSPT_Vector2:
        for (int i = 0; i < 2; ++i) osData << floats[i];
        break;
      case eSPT_Vector3:
        for (int i = 0; i < 3; ++i) osData << floats[i];
        break;
      case eSPT_Vector4:
        for (int i = 0; i < 4; ++i) osData << floats[i];
        break;
      case eSPT_Int:
        osData << ints[0];
        break;
      case eSPT_IVector2:
        for (int i = 0; i < 2; ++i) osData << ints[i];
        break;
      case eSPT_IVector3:
        for (int i = 0; i < 3; ++i) osData << ints[i];
        break;
      case eSPT_IVector4:
        for (int i = 0; i < 4; ++i) osData << ints[i];
        break;
      case eSPT_Color4:
        for (int i = 0; i < 4; ++i) osData << floats[i];
        break;
      case eSPT_Matrix3:
        for (int i = 0; i < 9; ++i) osData << floats[i];
        break;
      case eSPT_Matrix4:
        for (int i = 0; i < 16; ++i) osData << floats[i];
        break;
      case eSPT_Texture:
        osData << txtResLocator.GetResourceFile();
      }
    }


    QPointF pos = graphNode->GetItem()->pos();
    osMeta << (vkUInt32)i << vkVector2f(pos.x(), pos.y());
  }

  osData << (vkUInt16)inputs.size();
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
    osData << (vkUInt32)sgNodes.indexOf(inputNode)
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
    vkSGOutput *output = tempGraph.GetInput((vkSGShaderGraph::InputType)i);
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

  osData << (vkUInt16)attribs.size();
  for (Attrib &attr : attribs)
  {
    osData << attr.attrib
      << attr.nodeIdx
      << attr.nodeOutputIdx;
  }



  osData << (vkUInt16)2
    // binary gradient
    << vkString("blendOutWithBinaryGradient")
    << (vkUInt8)(tempGraph.IsBlendOutWithBinaryGradient() ? 1 : 0)
    // discard alpha
    << vkString("discardAlpha")
    << (vkUInt8)(tempGraph.IsDiscardAlpha() ? 1 : 0)
    << (vkUInt8)(tempGraph.GetDiscardAlphaCompareMode())
    << (float)tempGraph.GetDiscardAlphaThreshold();


  IFile *file = vkVFS::Get()->Open(m_resourceLocator.GetResourceFile(), eOM_Write, eTM_Binary);
  AssetWriter writer;
  writer.AddEntry("SHADER_GRAPH", "DATA", osData.GetSize(), osData.GetBuffer());
  writer.AddEntry("SHADER_GRAPH", "META_DATA", osMeta.GetSize(), osMeta.GetBuffer());

  writer.Output(file);
  file->Close();
}




void ShaderGraphView::CollectData(vkSGShaderGraph *graph, std::map<graph::Node*, vkSGNode*>& nodes)
{
  if (!graph)
  {
    return;
  }

  graph->Clear();
  nodes.clear();

  // create all nodes for the graph and assign all const values
  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {
    shadergraph::Node* node = static_cast<shadergraph::Node*>(m_scene->GetNode(i));
    if (node->GetType() != shadergraph::Node::eT_Node)
    {
      continue;
    }

    shadergraph::SGNode* sgNode = static_cast<shadergraph::SGNode*>(node);
    vkSGNode *vksgNode = sgNode->GetClass()->CreateInstance<vkSGNode>();
    nodes[sgNode] = vksgNode;

    for (size_t j = 0, jn = sgNode->GetNumberOfInputs(); j < jn; ++j)
    {
      float constFloat = sgNode->GetConstInput(j);
      vksgNode->SetInput(j, constFloat);
    }

    // the node has a resource name
    vkSGResourceNode *resNode = vkQueryClass<vkSGResourceNode>(vksgNode);
    if (resNode && sgNode->HasName())
    {
      resNode->SetResourceName(vkString((const char*)sgNode->GetName().toLatin1()));
    }

    graph->AddNode(vksgNode);
  }

  // create all connections
  for (size_t i = 0, in = m_scene->GetNumberOfConnections(); i < in; ++i)
  {
    graph::NodeConnection* connection = m_scene->GetConnection(i);
    if (nodes.find(connection->GetInputNode()) == nodes.end() || nodes.find(connection->GetOutputNode()) == nodes.end())
    {
      continue;
    }

    vkSGNode *inputNode = nodes[connection->GetInputNode()];
    vkSGNode *outputNode = nodes[connection->GetOutputNode()];

    vkSGOutput *output = outputNode->GetOutput(connection->GetOutputIdx());
    vkSGInput *input = inputNode->GetInput(connection->GetInputIdx());

    input->SetInput(output);
  }


  for (auto connection : m_shaderGraphNode->GetAllConnections())
  {
    if (nodes.find(connection->GetOutputNode()) == nodes.end())
    {
      continue;
    }
    vkSGNode *outputNode = nodes[connection->GetOutputNode()];
    vkSGOutput *output = outputNode->GetOutput(connection->GetOutputIdx());

    graph->SetInput((vkSGShaderGraph::InputType)connection->GetInputIdx(), output);
  }


  graph->SetBlendOutWithBinaryGradient(m_gui.cbBlendBinaryGradient->checkState() != 0);
  graph->SetDiscardAlpha(m_gui.cbDiscardAlpha->checkState() != 0);
  graph->SetDiscardAlpha((float)m_gui.sbDiscardAlphaThreshold->value(),
                         (vkCompareMode)m_gui.cbDiscardAlphaCompareMode->currentIndex());
}