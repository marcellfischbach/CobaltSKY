
#include <ShaderGraph/ShaderGraphView.hh>
#include <ShaderGraph/MetaData.hh>
#include <ShaderGraph/NodeSelector.hh>
#include <ShaderGraph/PreviewWidget.hh>
#include <ShaderGraph/ShaderEditor.hh>
#include <ShaderGraph/ShaderGraphScene.hh>
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <AssetManager/AssetWriter.hh>
#include <AssetManager/Utils.hh>
#include <Graph/Node.hh>
#include <Editor.hh>
#include <qgridlayout.h>
#include <qgraphicsitem.h>
#include <qgraphicswidget.h>
#include <qpainterpath.h>
#include <qevent.h>
#include <qtimer.h>
#include <qscrollbar.h>
#include <QMessageBox>
#include <QMimeData>
#include <qdom.h>

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>

class MyGraphicsView : public QGraphicsView
{
public:
  MyGraphicsView(ShaderGraphWidget *view)
    : QGraphicsView(view)
    , m_scrolling(false)
    , m_lastValidDrag(false)
    , m_view(view)
  {
    setAcceptDrops(true);
    setMouseTracking(true);
  }

protected:
  virtual void mousePressEvent(QMouseEvent *event)
  {
    if (event->button() == Qt::MidButton)
    {
      m_scrolling = true;
      m_lastPos = event->pos();
    }
    else
    {
      QGraphicsView::mousePressEvent(event);
    }
  }

  virtual void mouseReleaseEvent(QMouseEvent *event)
  {
    if (event->button() == Qt::MidButton)
    {
      m_scrolling = false;
    }
    else
    {
      QGraphicsView::mouseReleaseEvent(event);
    }
  }

  virtual void mouseMoveEvent(QMouseEvent *event)
  {
    if (m_scrolling)
    {
      QPoint dpos = event->pos() - m_lastPos;
      m_lastPos = event->pos();

      horizontalScrollBar()->setValue(horizontalScrollBar()->value() - dpos.x());
      verticalScrollBar()->setValue(verticalScrollBar()->value() - dpos.y());
    }
    else
    {
      QGraphicsView::mouseMoveEvent(event);
    }
  }

  virtual void wheelEvent(QWheelEvent *event)
  {
    int scroll = event->angleDelta().y();
    QMatrix m = matrix();
    if (scroll > 0)
    {
      m.scale(1.0f / 0.9f, 1.0f / 0.9f);
    }
    else
    {
      m.scale(0.9f, 0.9f);
    }
    setMatrix(m);
  }


  virtual void dragEnterEvent(QDragEnterEvent *event)
  {
    m_lastValidDrag = false;
    const QMimeData *data = event->mimeData();
    if (data->hasFormat("VALKYRIE/RESOURCE/TYPE"))
    {
      QByteArray type = data->data("VALKYRIE/RESOURCE/TYPE");
      const vkClass *resClass = vkClassRegistry::Get()->GetClass((const char*)type);
      m_lastValidDrag = resClass && resClass->IsInstanceOf<ITexture2D>();
    }
    if (m_lastValidDrag)
    {
      event->acceptProposedAction();
    }
  }
  virtual void dragMoveEvent(QDragMoveEvent *event)
  {
    if (m_lastValidDrag)
    {
      event->acceptProposedAction();
    }
  }

  virtual void dropEvent(QDropEvent *event)
  {
    const QMimeData *data = event->mimeData();
    if (m_lastValidDrag)
    {
      if (data->hasFormat("VALKYRIE/RESOURCE/TYPE") && data->hasFormat("VALKYRIE/RESOURCE/FILE"))
      {
        QByteArray type = data->data("VALKYRIE/RESOURCE/TYPE");
        const vkClass *resClass = vkClassRegistry::Get()->GetClass((const char*)type);
        QByteArray file= data->data("VALKYRIE/RESOURCE/FILE");
        if (resClass && resClass->IsInstanceOf<ITexture2D>())
        {
          m_view->AddTexture2D(vkResourceLocator(vkString((const char*)file)), event->pos());
        }

      }

      event->acceptProposedAction();
    }
    m_lastValidDrag = false;
  }


private:
  bool m_scrolling;
  QPoint m_lastPos;

  bool m_lastValidDrag;
  ShaderGraphWidget *m_view;
};


ShaderGraphWidget::ShaderGraphWidget(QWidget *parent)
  : QWidget(parent)
  , m_editorWidget(0)
{
  m_gui.setupUi(this);
  setMouseTracking(true);
  m_view = new MyGraphicsView(this);
  m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
  m_view->setContextMenuPolicy(Qt::CustomContextMenu);

  QGridLayout *layout = new QGridLayout(m_gui.wShaderGraphView);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_view);


  m_view->setBackgroundBrush(QBrush(QColor(32, 32, 32)));
  m_scene = new shadergraph::ShaderGraphScene();
  /*
  connect(m_scene, SIGNAL(NodeAdded(graph::Node*)), this, SLOT(NodeAdded(graph::Node*)));
  connect(m_scene, SIGNAL(NodeConnectedLooseInput(graph::Node*, int)), this, SLOT(NodeConnectedLooseInput(graph::Node*, int)));
  connect(m_scene, SIGNAL(NodesConnected(graph::Node*, int, graph::Node*, int)), this, SLOT(NodesConnected(graph::Node*, int, graph::Node*, int)));
  connect(m_scene, SIGNAL(NodeRemoved(graph::Node*)), this, SLOT(NodeRemoved(graph::Node*)));
  connect(m_scene, SIGNAL(NodeSelected(graph::Node*)), this, SLOT(NodeSelected(graph::Node*)));
  */
  m_view->setScene(m_scene);


  m_previewWidget = new shadergraph::PreviewWidget(this);
  layout = new QGridLayout(m_gui.wPreview);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_previewWidget);

  connect(m_view, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ViewRightClicked(const QPoint&)));

}


ShaderGraphWidget::~ShaderGraphWidget()
{
}


void ShaderGraphWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;

  vkResourceLocator metaLocator = vkResourceLocator(m_resourceLocator.GetResourceFile(), "meta");

  vkSGShaderGraph *shaderGraph = vkResourceManager::Get()->GetOrLoad<vkSGShaderGraph>(m_resourceLocator);
  ShaderGraphMetaData *metaData = vkResourceManager::Get()->Load<ShaderGraphMetaData>(metaLocator);
  bool nullShader = !shaderGraph;
  if (nullShader)
  {
    shaderGraph = new vkSGShaderGraph();
  }
  Setup(shaderGraph, metaData);

  VK_RELEASE(metaData);

  Compile();


  m_previewWidget->SetMaterial(m_shaderGraph);
  m_previewWidget->repaint();
}


void ShaderGraphWidget::Setup(vkSGShaderGraph *shaderGraph, ShaderGraphMetaData *metaData)
{

  // create a full copy of the src shader
  m_shaderGraph = new vkSGShaderGraph();
  SyncGraph(shaderGraph, m_shaderGraph);
  m_scene->Setup(m_shaderGraph, metaData);
}
/*

  // now create the graph node for the shader
  m_shaderGraphNode = new shadergraph::SGShaderGraphNode(m_shaderGraph);
  m_shaderGraphNode->Initialize();
  m_scene->AddNode(m_shaderGraphNode);

  if (metaData)
  {
    vkVector2f graphPos = metaData->GetGraphPosition();
    m_shaderGraphNode->SetPosition(QPointF(graphPos.x, graphPos.y));
  }

  // now create the graph nodes for the shader's nodes
  std::map<vkSGNode*, graph::Node*> nodes;
  for (size_t i = 0, in = m_shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *sgNode = m_shaderGraph->GetNode(i);
    vkVector2f nodePosition = metaData ? metaData->GetNodePosition(i) : vkVector2f(0.0f, 0.0f);
    graph::Node *gnode = AddNode(sgNode, nodePosition);
    nodes[sgNode] = gnode;
  }
  fflush(stdout);


  for (size_t i = 0, in = m_shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *inputNode = m_shaderGraph->GetNode(i);
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
    vkSGOutput *output = m_shaderGraph->GetInput((vkSGShaderGraph::InputType)i);
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


  m_scene->SetSilent(false);
}
*/


void ShaderGraphWidget::SyncGraph(const vkSGShaderGraph *src, vkSGShaderGraph *dst)
{
  dst->SetBlendOutWithBinaryGradient(src->IsBlendOutWithBinaryGradient());
  dst->SetDiscardAlpha(src->IsDiscardAlpha());
  dst->SetDiscardAlpha(src->GetDiscardAlphaThreshold(),
                       src->GetDiscardAlphaCompareMode());

  SyncNodes(src, dst);
}

void ShaderGraphWidget::SyncNodes(const vkSGShaderGraph *src, vkSGShaderGraph *dst)
{
  dst->Clear();

  for (vkSize i=0, in=src->GetNumberOfTotalNodes(); i<in; ++i)
  {
    vkSGNode *node = Copy(src->GetNode(i));
    dst->AddNode(node);
  }

  for (vkSize i=0, in=src->GetNumberOfTotalNodes(); i<in; ++i)
  {
    const vkSGNode *srcNode = src->GetNode(i);
    vkSGNode *dstNode = dst->GetNode(i);

    for (vkSize j=0, jn=srcNode->GetNumberOfInputs(); j<jn; ++j)
    {
      const vkSGInput *srcInput = srcNode->GetInput(j);
      vkSGInput *dstInput = dstNode->GetInput(j);
      if (srcInput->CanInputConst())
      {
        dstInput->SetConst(srcInput->GetConst());
      }
      if (srcInput->CanInputNode())
      {
        const vkSGOutput *srcOutput = srcInput->GetInput();
        if (srcOutput)
        {
          const vkSGNode *srcOutputNode = srcOutput->GetNode();
          int srcOutputNodeIndex = src->GetIndexOfNode(srcOutputNode);
          if (srcOutputNodeIndex != -1)
          {
            vkSGNode *dstOutputNode = dst->GetNode(srcOutputNodeIndex);
            if (dstOutputNode)
            {
              vkSGOutput *dstOutput = dstOutputNode->GetOutput(srcOutput->GetIdx());
              if (dstOutput)
              {
                dstInput->SetInput(dstOutput);
              }
            }
          }
        }
      }
    }
  }

  for (int i=vkSGShaderGraph::eIT_START; i<vkSGShaderGraph::eIT_COUNT; ++i)
  {
    vkSGShaderGraph::InputType inputType = (vkSGShaderGraph::InputType)i;

    const vkSGOutput *srcOutput = src->GetInput(inputType);
    if (srcOutput)
    {
      const vkSGNode *srcOutputNode = srcOutput->GetNode();
      int srcOutputNodeIdx = src->GetIndexOfNode(srcOutputNode);
      if (srcOutputNodeIdx != -1)
      {
        vkSGNode *dstOutputNode = dst->GetNode(srcOutputNodeIdx);
        if (dstOutputNode)
        {
          vkSGOutput *dstOutput = dstOutputNode->GetOutput(srcOutput->GetIdx());
          if (dstOutput)
          {
            dst->SetInput(inputType, dstOutput);
          }
        }
      }
    }
  }
}




vkSGNode *ShaderGraphWidget::Copy(const vkSGNode *node)
{
  if (!node)
  {
    return 0;
  }

  vkSGNode *newNode = node->GetClass()->CreateInstance<vkSGNode>();

  vkSGResourceNode *newResourceNode = vkQueryClass<vkSGResourceNode>(newNode);
  const vkSGResourceNode *resourceNode = vkQueryClass<const vkSGResourceNode>(node);
  if (resourceNode && newResourceNode)
  {
    newResourceNode->SetResourceName(resourceNode->GetResourceName());
    memcpy (newResourceNode->GetDefaultFloats(), resourceNode->GetDefaultFloats(), sizeof(float) * 16);
    memcpy (newResourceNode->GetDefaultInts(), resourceNode->GetDefaultInts(), sizeof(int) * 16);
    newResourceNode->GetDefaultTextureResource() = resourceNode->GetDefaultTextureResource();
  }
  return newNode;
}



void  ShaderGraphWidget::keyReleaseEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Space)
  {
    popupNodeSelector();
  }
  else if (event->key() == Qt::Key_Delete)
  {
    /*
    graph::Node *selectedNode = m_scene->GetSelectedNode();
    if (!selectedNode)
    {
      return;
    }
    if (static_cast<shadergraph::SGNode*>(selectedNode)->GetType() == shadergraph::SGNode::eT_Node)
    {
      m_scene->RemoveSelectedNode();
    }
    */
  }
}

void ShaderGraphWidget::popupNodeSelector()
{
  NodeSelector *selector = new NodeSelector(this);
  connect(selector, SIGNAL(addNode(const vkClass*)), this, SLOT(AddNode(const vkClass*)));

  QPoint pos = m_view->mapFromScene(QPointF(m_newNodePosition.x, m_newNodePosition.y));
  pos = mapToGlobal(pos);
  selector->move(pos);
  selector->setVisible(true);
}

void ShaderGraphWidget::ViewRightClicked(const QPoint &p)
{
  QPointF scenePos = m_view->mapToScene(p);
  m_newNodePosition = vkVector2f(scenePos.x(), scenePos.y());

  popupNodeSelector();
}

graph::GraphNode *ShaderGraphWidget::AddNode(const vkClass *clazz)
{
  vkSGNode *node = clazz->CreateInstance<vkSGNode>();
  graph::GraphNode *sgNode = AddNode(node, m_newNodePosition);
  VK_RELEASE(node);
  return sgNode;
}

void ShaderGraphWidget::AddTexture2D(const vkResourceLocator &locator, const QPoint &pos)
{
  vkSGTexture2D *txt2D = vkSGTexture2D::GetStaticClass()->CreateInstance<vkSGTexture2D>();
  txt2D->GetDefaultTextureResource() = locator;
  txt2D->SetResourceName(vkString((const char*)assetmanager::GetNameFromResource(locator).toLatin1()));


  QPointF scenePos = m_view->mapToScene(pos);

  AddNode(txt2D, vkVector2f(scenePos.x(), scenePos.y()));
  VK_RELEASE(txt2D);
}

graph::GraphNode *ShaderGraphWidget::AddNode(vkSGNode *node, const vkVector2f &pos)
{
  if (!node)
  {
    return 0;
  }
  shadergraph::SGNode *sgNode = new shadergraph::SGNode(node);

  m_scene->AddNode(sgNode);
  sgNode->setPos(QPointF(pos.x, pos.y));
  return sgNode;
}



bool ShaderGraphWidget::Compile()
{
  if (!vkEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraph))
  {
    //    for (auto it : nodes)
    //    {
    //      printf("Node: %s\n", it.second->GetValidationMessage().c_str());
    //    }
    return false;
  }

  // the material itself will not change, but by setting the material again
  // the parameters of the material instance will update (maybe parameters were added)
  m_previewWidget->SetMaterial(m_shaderGraph);
  m_previewWidget->repaint();
  return true;
}

void ShaderGraphWidget::on_pbCompile_clicked(bool)
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

void ShaderGraphWidget::on_pbSave_clicked(bool)
{
  if (!Compile())
  {
    QMessageBox::StandardButton button = QMessageBox::question(this, Editor::Get().GetTitleName(),
                                                               tr("The material could not be compiled.\nStill want to save the material?"),
                                                               QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No));
    if (button == QMessageBox::No)
    {
      return;
    }
  }

  vkSGShaderGraph *shaderGraph = vkResourceManager::Get()->GetOrLoad<vkSGShaderGraph>(m_resourceLocator);
  if (shaderGraph)
  {
    SyncGraph(m_shaderGraph, shaderGraph);
    vkEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph);
  }



  if (m_resourceLocator.GetResourceFile().length() == 0)
  {
    printf("Invalid resource\n");
    return;
  }

  Compile();


  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement metaElement = doc.createElement("meta");
  QDomElement shadergraphElement = doc.createElement("shaderGraph");
  QDomElement nodesElement = doc.createElement("nodes");
  QDomElement inputsElement = doc.createElement("inputs");
  QDomElement attributesElement = doc.createElement("attributes");
  QDomElement metaShadergraphElement = doc.createElement("shaderGraphMeta");
  QDomElement metaNodesElement = doc.createElement("nodes");



  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  assetElement.appendChild(metaElement);

  dataElement.appendChild(shadergraphElement);
  shadergraphElement.appendChild(nodesElement);
  shadergraphElement.appendChild(inputsElement);
  shadergraphElement.appendChild(attributesElement);

  metaElement.appendChild(metaShadergraphElement);
  metaShadergraphElement.appendChild(metaNodesElement);




  QPointF shaderGraphPos = m_shaderGraphNode->pos();


  QDomElement metaShaderGraphShaderGraphElement = doc.createElement("shaderGraph");
  metaShaderGraphShaderGraphElement.
    appendChild(doc.createElement("pos")).
    appendChild(doc.createTextNode(QString::asprintf("%f, %f", shaderGraphPos.x(), shaderGraphPos.y())));

  metaShadergraphElement.appendChild(metaShaderGraphShaderGraphElement);


  QVector<vkSGNode*> sgNodes;
  QVector<vkSGInput*> inputs;
  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {
    graph::GraphNode *graphNode = m_scene->GetNode(i);
    if (graphNode == m_shaderGraphNode)
    {
      continue;
    }

    vkSGNode *sgNode = static_cast<shadergraph::SGNode*>(graphNode)->GetNode();
    for (size_t j = 0, jn = sgNode->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput* input = sgNode->GetInput(j);
      inputs.push_back(input);
    }

    sgNodes.append(sgNode);
    vkUInt32 index = sgNodes.indexOf(sgNode);

    QDomElement nodeElement = doc.createElement("node");
    nodeElement.setAttribute("id", index);
    nodeElement.setAttribute("class", QString(sgNode->GetClass()->GetName().c_str()));

    vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(sgNode);
    if (resourceNode)
    {

      float *floats = resourceNode->GetDefaultFloats();
      int *ints = resourceNode->GetDefaultInts();
      vkResourceLocator &txtResLocator = resourceNode->GetDefaultTextureResource();

      QDomElement resourceElement = doc.createElement("resource");
      resourceElement.setAttribute("name", QString(resourceNode->GetResourceName().c_str()));

      switch (resourceNode->GetResourceType())
      {
      case eSPT_Float:
        resourceElement.
          appendChild(doc.createElement("float")).
          appendChild(doc.createTextNode(QString::asprintf("%f", floats[0])));
        break;
      case eSPT_Vector2:
        resourceElement.
          appendChild(doc.createElement("float2")).
          appendChild(doc.createTextNode(QString::asprintf("%f, %f", floats[0], floats[1])));
        break;
      case eSPT_Vector3:
        resourceElement.
          appendChild(doc.createElement("float3")).
          appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f", floats[0], floats[1], floats[2])));
        break;
      case eSPT_Vector4:
        resourceElement.
          appendChild(doc.createElement("float4")).
          appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", floats[0], floats[1], floats[2], floats[3])));
        break;
      case eSPT_Int:
        resourceElement.
          appendChild(doc.createElement("int")).
          appendChild(doc.createTextNode(QString::asprintf("%d", ints[0])));
        break;
      case eSPT_IVector2:
        resourceElement.
          appendChild(doc.createElement("int2")).
          appendChild(doc.createTextNode(QString::asprintf("%d, %d", ints[0], ints[1])));
        break;
      case eSPT_IVector3:
        resourceElement.
          appendChild(doc.createElement("int3")).
          appendChild(doc.createTextNode(QString::asprintf("%d, %d, %d", ints[0], ints[1], ints[2])));
        break;
      case eSPT_IVector4:
        resourceElement.
          appendChild(doc.createElement("int4")).
          appendChild(doc.createTextNode(QString::asprintf("%d, %d, %d, %d", ints[0], ints[1], ints[2], ints[3])));
        break;

      case eSPT_Color4:
         resourceElement.
           appendChild(doc.createElement("color4")).
           appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", floats[0], floats[1], floats[2], floats[3])));
        break;
      case eSPT_Matrix3:
        {
          QDomElement valueElement = doc.createElement("mat3");
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f", floats[0], floats[1], floats[2])));
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f", floats[4], floats[5], floats[6])));
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f", floats[8], floats[9], floats[10], floats[11])));
          resourceElement.appendChild(valueElement);
        }
        break;
      case eSPT_Matrix4:
        {
          QDomElement valueElement = doc.createElement("mat4");
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", floats[0], floats[1], floats[2], floats[3])));
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", floats[4], floats[5], floats[6], floats[7])));
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", floats[8], floats[9], floats[10], floats[11])));
          valueElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", floats[12], floats[13], floats[14], floats[15])));
          resourceElement.appendChild(valueElement);
        }
        break;
      case eSPT_Texture:
        resourceElement.
          appendChild(doc.createElement("locator")).
          appendChild(doc.createTextNode(QString(txtResLocator.GetResourceFile().c_str())));
      }
      nodeElement.appendChild(resourceElement);
    }

    nodesElement.appendChild(nodeElement);

    QPointF pos = graphNode->pos();
    QDomElement metaNodeElement = doc.createElement("node");
    metaNodeElement.setAttribute("id", index);
    metaNodeElement.
      appendChild(doc.createElement("pos")).
      appendChild(doc.createTextNode(QString::asprintf("%f, %f", pos.x(), pos.y())));

    metaNodesElement.appendChild(metaNodeElement);
  }

  for (unsigned i = 0; i < vkSGShaderGraph::eIT_COUNT; ++i)
  {

    vkSGShaderGraph::InputType inputType = (vkSGShaderGraph::InputType)i;
    vkSGOutput *output = m_shaderGraph->GetInput(inputType);
    if (output)
    {
      vkSGNode *outputNode = output->GetNode();

      QDomElement shadergraphInputElement = doc.createElement("shaderGraph");
#define INPUT(input) if (inputType == vkSGShaderGraph::eIT_##input) shadergraphInputElement.setAttribute("input", #input)
      INPUT(Diffuse);
      INPUT(Alpha);
      INPUT(Roughness);
      INPUT(Normal);
#undef INPUT


      QDomElement nodeElement = doc.createElement("node");
      nodeElement.setAttribute("id", sgNodes.indexOf(outputNode));
      nodeElement.setAttribute("output", output->GetIdx());
      shadergraphInputElement.appendChild(nodeElement);
      inputsElement.appendChild(shadergraphInputElement);
    }
  }

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

    QDomElement nodeElement = doc.createElement("node");
    nodeElement.setAttribute("id", sgNodes.indexOf(inputNode));
    nodeElement.setAttribute("input", inputIdx);

    if (output) 
    {
      QDomElement refNodeElement = doc.createElement("node");
      refNodeElement.setAttribute("id", sgNodes.indexOf(outputNode));
      refNodeElement.setAttribute("output", outputIdx);
      nodeElement.appendChild(refNodeElement);
    }
    else
    {
      QDomElement refFloatElement = doc.createElement("float");
      refFloatElement.appendChild(doc.createTextNode(QString::asprintf("%f", input->GetConst())));
      nodeElement.appendChild(refFloatElement);
    }
    inputsElement.appendChild(nodeElement);
  }

  // 
  // write the attributes of the shader graph
  QDomElement blendOutWithBinaryGradientElement = doc.createElement("blendOutWithBinaryGradient");
  blendOutWithBinaryGradientElement.setAttribute("enabled", shaderGraph->IsBlendOutWithBinaryGradient());
  attributesElement.appendChild(blendOutWithBinaryGradientElement);


  QDomElement discardAlphaElement = doc.createElement("discardAlpha");
  QDomElement thresholdElement = doc.createElement("threshold");
  thresholdElement.appendChild(doc.createTextNode(QString::asprintf("%f", shaderGraph->GetDiscardAlphaThreshold())));
  QDomElement modeElement = doc.createElement("mode");
#define CMP(cmp) (shaderGraph->GetDiscardAlphaCompareMode() == eCM_##cmp) modeElement.setAttribute("mode", #cmp)
  if CMP(LessOrEqual);
  else if CMP(GreaterOrEqual);
  else if CMP(Less);
  else if CMP(Greater);
  else if CMP(Equal);
  else if CMP(NotEqual);
  else if CMP(Never);
  else if CMP(Always);
  discardAlphaElement.appendChild(thresholdElement);
  discardAlphaElement.appendChild(modeElement);
  attributesElement.appendChild(discardAlphaElement);


  QString xml = doc.toString(2);

  vkString absolutPath = vkVFS::Get()->GetAbsolutPath(m_resourceLocator.GetResourceFile());

  QFile file(QString(absolutPath.c_str()));
  file.open(QIODevice::ReadWrite);
  file.write(xml.toLatin1());
  file.close();
  return;
#if 0
  /*
  vkSGShaderGraph tempGraph;
  std::map<graph::Node*, vkSGNode*> nodes;
  CollectData(&tempGraph, nodes);
  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(&tempGraph))
  {
    return;
  }
  std::map<vkSGNode*, graph::Node*> nodesInv;
  ::invert(nodes, nodesInv);

  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {

    sgNodes.append(tempGraph.GetNode(i));
  }
  */

  QVector<vkSGNode*> sgNodes;
  QVector<vkSGInput*> inputs;

  /*
  for (size_t i = 0, in = sgNodes.size(); i < in; ++i)
  {
    vkSGNode* node = sgNodes[i];
    for (size_t j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput* input = node->GetInput(j);
      inputs.push_back(input);
    }
  }
  */


  QPointF shaderGraphPos = m_shaderGraphNode->GetItem()->pos();
  vkAssetOutputStream osMeta;
  osMeta << (vkUInt32)VK_VERSION(1, 0, 0);
  osMeta << vkVector2f(shaderGraphPos.x(), shaderGraphPos.y());

  vkAssetOutputStream osData;
  osData << (vkUInt32)VK_VERSION(1, 0, 0);
  osData << (vkUInt16)(m_scene->GetNumberOfNodes()-1); // one node is the shader graph itself
  osMeta << (vkUInt16)(m_scene->GetNumberOfNodes()-1);
  for (size_t i = 0, in = m_scene->GetNumberOfNodes(); i < in; ++i)
  {
    graph::Node *graphNode = m_scene->GetNode(i);
    if (graphNode == m_shaderGraphNode)
    {
      continue;
    }

    vkSGNode *sgNode = static_cast<shadergraph::SGNode*>(graphNode)->GetNode();
    for (size_t j = 0, jn = sgNode->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput* input = sgNode->GetInput(j);
      inputs.push_back(input);
    }

    sgNodes.append(sgNode);
    vkUInt32 index = sgNodes.indexOf(sgNode);


    osData << index
           << vkString(sgNode->GetClass()->GetName());

    vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(sgNode);
    if (resourceNode)
    {
      float *floats = resourceNode->GetDefaultFloats();
      int *ints = resourceNode->GetDefaultInts();
      vkResourceLocator &txtResLocator = resourceNode->GetDefaultTextureResource();
      osData << resourceNode->GetResourceName();
      switch (resourceNode->GetResourceType())
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
    osMeta << index
           << vkVector2f(pos.x(), pos.y());
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
         << (vkUInt8)(m_shaderGraph->IsBlendOutWithBinaryGradient() ? 1 : 0)
            // discard alpha
         << vkString("discardAlpha")
         << (vkUInt8)(m_shaderGraph->IsDiscardAlpha() ? 1 : 0)
         << (vkUInt8)(m_shaderGraph->GetDiscardAlphaCompareMode())
         << (float)m_shaderGraph->GetDiscardAlphaThreshold();


  IFile *file = vkVFS::Get()->Open(m_resourceLocator.GetResourceFile(), eOM_Write, eTM_Binary);
  assetmanager::AssetWriter writer;
  writer.AddEntry("SHADER_GRAPH", "DATA", osData.GetSize(), osData.GetBuffer());
  writer.AddEntry("SHADER_GRAPH", "META_DATA", osMeta.GetSize(), osMeta.GetBuffer());

  writer.Output(file);
  file->Close();
#endif
}




ShaderGraphView::ShaderGraphView (QWidget *parent)
  : EditorView ()
{
  m_shaderGraphWidget = new ShaderGraphWidget(parent);
  SetWidget(m_shaderGraphWidget);
  SetCanClose(true);
}

ShaderGraphView::~ShaderGraphView()
{
  m_shaderGraphWidget->deleteLater();
}

void ShaderGraphView::Initialize()
{
  m_shaderGraphWidget->Set(GetResourceLocator());
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()) + QString("(ShaderGraph)"));
}

bool ShaderGraphView::Close()
{
  return true;
}



ShaderGraphViewFactory::ShaderGraphViewFactory()
  : IEditorViewFactory()
{

}

ShaderGraphViewFactory::~ShaderGraphViewFactory()
{

}


bool ShaderGraphViewFactory::CanEdit(const vkResourceLocator &resourceLocator, IObject *obj)
{
  if (vkQueryClass<vkSGShaderGraph>(obj))
  {
    return true;
  }

  return false;
}

EditorView *ShaderGraphViewFactory::CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj)
{
  ShaderGraphView *view = new ShaderGraphView(parent);
  view->SetEditorObject(obj);
  view->SetResourceLocator(resourceLocator);
  view->Initialize();
  return view;
}


