
#include <editors/shadergrapheditor/shadergrapheditorwidget.hh>
#include <editors/shadergrapheditor/shadergrapheditor.hh>
#include <editors/shadergrapheditor/shadergrapheditormeta.hh>
#include <editors/shadergrapheditor/shadergrapheditornode.hh>
#include <editors/shadergrapheditor/shadergrapheditortoolboxmodel.hh>

#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>


#include <mimehelper.hh>
#include <editor.hh>
#include <project/project.hh>

#include <cobalt/csengine.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/shadergraph/cssgdefaultconsolelogger.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/shadergraph/cssgtexture2d.hh>
#include <cobalt/core/csvfs.hh>
#include <csfile/csffile.hh>

#include <QDropEvent>
#include <QFile>
#include <QImage>


ShaderGraphEditorWidget::ShaderGraphEditorWidget(ShaderGraphEditor *editor)
  : QWidget()
  , m_editor(editor)
  , m_shaderGraph(0)
  , m_shaderGraphCopy(new csSGShaderGraph())
  , m_updateGuard(false)
{
  m_gui.setupUi(this);
  on_nodeGraph_ScaleChanged(1.0f);

  csSGShaderGraph *shaderGraph = new csSGShaderGraph();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(shaderGraph);
  m_gui.nodeGraph->AddNode(shaderGraphNode);
}

ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  CS_RELEASE(m_shaderGraph);
  CS_RELEASE(m_shaderGraphCopy);
}

csSGShaderGraph *ShaderGraphEditorWidget::SetShaderGraph(csSGShaderGraph *shaderGraph, ShaderGraphEditorMeta *meta)
{
  CS_SET(m_shaderGraph, shaderGraph);
  if (!m_shaderGraph)
  {
    return 0;
  }

  m_updateGuard = true;
  m_shaderGraphCopy = m_shaderGraph->Copy(m_shaderGraphCopy);

  m_gui.nodeGraph->Clear();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(m_shaderGraphCopy);
  csVector2f shaderGraphNodePos = meta->GetPos();
  shaderGraphNode->SetLocation(QPointF(shaderGraphNodePos.x, shaderGraphNodePos.y));
  m_gui.nodeGraph->AddNode(shaderGraphNode);

  std::map<csSGNode *, ShaderGraphEditorNode*> mapping;
  for (csSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    csSGNode *node = m_shaderGraphCopy->GetNode(i);

    ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(node);
    csVector2f nodePos = meta->GetPos(i);
    editorNode->SetLocation(QPointF(nodePos.x, nodePos.y));
    m_gui.nodeGraph->AddNode(editorNode);

    mapping[node] = editorNode;
  }

  for (csSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    csSGNode *node = m_shaderGraphCopy->GetNode(i);
    for (csSize j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      csSGInput* input = node->GetInput(j);
      csSGOutput *output = input->GetInput();
      if (output)
      {
        csSGNode *outputNode = output->GetNode();

        ShaderGraphEditorNode* editorNode = mapping[node];
        ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

        NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
        NodeGraphNodeAnchor *inputAnchor = editorNode->GetInputAnchor(input->GetIdx());

        m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
      }
    }
  }

  for (csSize i = 0; i < csSGShaderGraph::eIT_COUNT; ++i)
  {
    csSGShaderGraph::InputType inputType = (csSGShaderGraph::InputType)i;
    csSGOutput *output = m_shaderGraphCopy->GetInput(inputType);
    if (output)
    {
      csSGNode *outputNode = output->GetNode();
      ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

      NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
      NodeGraphNodeAnchor *inputAnchor = shaderGraphNode->GetInputAnchor(i);

      m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
    }

  }
  m_updateGuard = false;
  return m_shaderGraphCopy;
}


void ShaderGraphEditorWidget::RepaintGraph()
{
  m_gui.nodeGraph->repaint();
}


void ShaderGraphEditorWidget::on_nodeGraph_CheckConnection(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB, NodeGraphVetoEvent *veto)
{
  if (!anchorA
    || !anchorB
    || anchorA->GetType() == anchorB->GetType()
    || anchorA->GetNode() == anchorB->GetNode())
  {
    veto->Veto();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_AboutToConnect(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (anchorA->GetType() == eNGNPT_Input)
  {
    m_gui.nodeGraph->DisconnectAll(anchorA);
  }
  if (anchorB->GetType() == eNGNPT_Input)
  {
    m_gui.nodeGraph->DisconnectAll(anchorB);
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_Connected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (m_updateGuard)
  {
    return;
  }
  NodeGraphNodeAnchor *outputAnchor = anchorA->GetType() == eNGNPT_Output ? anchorA : anchorB;
  NodeGraphNodeAnchor *inputAnchor = anchorA->GetType() == eNGNPT_Input ? anchorA : anchorB;

  ShaderGraphEditorNode *editorNodeOutput = static_cast<ShaderGraphEditorNode*>(outputAnchor->GetNode());
  ShaderGraphEditorNode *editorNodeInput = static_cast<ShaderGraphEditorNode*>(inputAnchor->GetNode());

  if (editorNodeInput->GetShaderGraph())
  {
    // make the connection to the shader graph
    csSGNode *nodeOutput = editorNodeOutput->GetSGNode();
    csSGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    csSGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((csSGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), output);
  }
  else
  {
    // make the connection between two nodes
    csSGNode *nodeOutput = editorNodeOutput->GetSGNode();
    csSGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    csSGNode *nodeInput = editorNodeInput->GetSGNode();
    csSGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());

    input->SetInput(output);
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_Disconnected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (m_updateGuard)
  {
    return;
  }

  printf("Disconnected: %p %p\n", anchorA, anchorB);
  NodeGraphNodeAnchor *inputAnchor = anchorA->GetType() == eNGNPT_Input ? anchorA : anchorB;

  ShaderGraphEditorNode *editorNodeInput = static_cast<ShaderGraphEditorNode*>(inputAnchor->GetNode());

  if (editorNodeInput->GetShaderGraph())
  {
    // make the connection to the shader graph
    csSGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((csSGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), 0);
  }
  else
  {
    // make the connection between two nodes
    csSGNode *nodeInput = editorNodeInput->GetSGNode();
    csSGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());
    input->SetInput(0);
  }
}



void ShaderGraphEditorWidget::on_nodeGraph_CheckLooseDisconnect(NodeGraphNodeAnchor *anchor, NodeGraphVetoEvent *veto)
{
  if (anchor->GetType() == eNGNPT_Output)
  {
    veto->Veto();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_ScaleChanged(float scale)
{
  int iscale = scale * 100.0f;
  m_gui.lblScale->setText(QString("%1%").arg(iscale));

}


void ShaderGraphEditorWidget::on_nodeGraph_CheckDrag(const QDropEvent *event, NodeGraphAcceptEvent *accept)
{
  // check the mime from the toolbox
  if (event->mimeData()->hasFormat(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME))
  {
    accept->Accept();
    return;
  }
  else
  {

    const QMimeData *data = event->mimeData();
    if (!data)
    {
      return;
    }

    const csClass *cls = MimeHelper::GetClass(data);
    if (!cls)
    {
      return;
    }

    if (!cls->IsInstanceOf(iTexture2D::GetStaticClass()))
    {
      if (!MimeHelper::HasResourceLocator(data))
      {
        return;
      }
    }

    accept->Accept();
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_DragDropped(const QDropEvent *event)
{
  ShaderGraphEditorNode *editorNode = 0;

  if (event->mimeData()->hasFormat(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME))
  {
    QByteArray ba = event->mimeData()->data(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME);
    QDataStream stream(&ba, QIODevice::ReadOnly);
    QString className;
    stream >> className;
    const csClass *cls = csClassRegistry::Get()->GetClass(std::string((const char*)className.toLatin1()));
    if (!cls)
    {
      return;
    }

    csSGNode *node = cls->CreateInstance<csSGNode>();
    if (!node)
    {
      return;
    }

    m_shaderGraphCopy->AddNode(node);

    editorNode = new ShaderGraphEditorNode(node);
  }
  else
  {


    const QMimeData *data = event->mimeData();
    if (!data)
    {
      return;
    }

    const csClass *cls = MimeHelper::GetClass(data);
    if (!cls)
    {
      return;
    }

    if (!cls->IsInstanceOf(iTexture2D::GetStaticClass()))
    {
      if (!MimeHelper::HasResourceLocator(data))
      {
        return;
      }
    }
    csResourceLocator locator = MimeHelper::GetResourceLocator(data);

    csSGTexture2D *txtNode = new csSGTexture2D();
    txtNode->SetDefaultTextureResource(locator);
    txtNode->SetResourceName(ExtractName(locator));

    m_shaderGraphCopy->AddNode(txtNode);

    editorNode = new ShaderGraphEditorNode(txtNode);
  }

  if (editorNode)
  {
    m_gui.nodeGraph->AddNode(editorNode);
    editorNode->SetLocation(m_gui.nodeGraph->GetLocalCoordinate(event->pos()));
    m_gui.nodeGraph->repaint();
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_RequestRemoveNode(QList<NodeGraphNode*> nodes, NodeGraphVetoEvent *veto)
{
  for (NodeGraphNode* node : nodes)
  {
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(node);
    if (editorNode->GetShaderGraph())
    {
      veto->Veto();
      return;
    }
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_NodeRemoved(NodeGraphNode* node)
{
  if (!node)
  {
    return;
  }
  printf("RemoveNode: %p\n", node);
  ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(node);
  csSGNode *sgNode = editorNode->GetSGNode();
  if (!sgNode)
  {
    m_shaderGraphCopy->RemoveNode(sgNode);
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_SelectionChanged(const QList<NodeGraphNode*> &nodes)
{
  QList<ShaderGraphEditorNode*> editorNodes;
  for (NodeGraphNode *node : nodes)
  {
    editorNodes.push_back(static_cast<ShaderGraphEditorNode*>(node));
  }

  emit SelectionChanged(editorNodes);
}

void ShaderGraphEditorWidget::on_pbApply_clicked()
{
  Apply();
}


void ShaderGraphEditorWidget::on_pbSave_clicked()
{
  if (!Apply())
  {
    return;
  }
  csfFile file;

  csfEntry *assetEntry= file.CreateEntry("asset");
  csfEntry *dataEntry = file.CreateEntry("data");
  csfEntry *shaderGraphEntry = file.CreateEntry("shaderGraph");
  csfEntry *metaEntry = file.CreateEntry("meta");
  csfEntry *shaderGraphMetaEntry = file.CreateEntry("shaderGraphMeta");

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  assetEntry->AddChild(metaEntry);
  dataEntry->AddChild(shaderGraphEntry);
  metaEntry->AddChild(shaderGraphMetaEntry);


  csfEntry *nodesEntry = file.CreateEntry("nodes");
  shaderGraphEntry->AddChild(nodesEntry);

  csfEntry *inputsEntry = file.CreateEntry("inputs");
  shaderGraphEntry->AddChild(inputsEntry);

  csfEntry *metaNodesEntry = file.CreateEntry("nodes");
  shaderGraphMetaEntry->AddChild(metaNodesEntry);

  size_t nodeId = 0;
  std::map<csSGNode*, size_t> nodeMap;
  for (size_t i = 0, in = m_gui.nodeGraph->GetNumberOfNodes(); i < in; ++i)
  {
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(m_gui.nodeGraph->GetNode(i));
    csSGNode *node = editorNode->GetSGNode();
    if (node)
    {

      nodeMap[node] = nodeId;

      csfEntry *nodeEntry = file.CreateEntry("node");
      nodeEntry->AddAttribute("id", std::to_string(nodeId));
      nodeEntry->AddAttribute("class", node->GetClass()->GetName());
      nodesEntry->AddChild(nodeEntry);

      csSGResourceNode *resourceNode = csQueryClass<csSGResourceNode>(node);
      if (resourceNode)
      {
        csfEntry *resourceEntry = file.CreateEntry("resource");
        resourceEntry->AddAttribute("name", resourceNode->GetResourceName());
        nodeEntry->AddChild(resourceEntry);

        csfEntry *valueEntry = 0;
        switch (resourceNode->GetResourceType())
        {
        case eSPT_Float:
          valueEntry = file.CreateEntry("float");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          break;
        case eSPT_Vector2:
          valueEntry = file.CreateEntry("float2");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          break;
        case eSPT_Vector3:
          valueEntry = file.CreateEntry("float3");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[2]));
          break;
        case eSPT_Vector4:
          valueEntry = file.CreateEntry("float4");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[2]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[3]));
          break;
        case eSPT_Int:
          valueEntry = file.CreateEntry("int");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          break;
        case eSPT_IVector2:
          valueEntry = file.CreateEntry("int2");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[1]));
          break;
        case eSPT_IVector3:
          valueEntry = file.CreateEntry("int3");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[2]));
          break;
        case eSPT_IVector4:
          valueEntry = file.CreateEntry("int4");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[2]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[3]));
          break;
        case eSPT_Color4:
          valueEntry = file.CreateEntry("color4");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[2]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[3]));
          break;
        case eSPT_Matrix3:
          break;
        case eSPT_Matrix4:
          break;
        case eSPT_Texture:
          valueEntry = file.CreateEntry("locator");
          valueEntry->AddAttribute("locator", resourceNode->GetDefaultTextureResource().GetResourceFile());
          break;
        }
        if (valueEntry)
        {
          resourceEntry->AddChild(valueEntry);
        }
      }
      csfEntry *nodeEntry1 = file.CreateEntry("node");
      nodeEntry1->AddAttribute("id", std::to_string(nodeId));
      metaNodesEntry->AddChild(nodeEntry1);

      csfEntry *posEntry = file.CreateEntry("pos");
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().x()));
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().y()));
      nodeEntry1->AddChild(posEntry);

      nodeId++;
    }
    csSGShaderGraph *shaderGraph = editorNode->GetShaderGraph();
    if (shaderGraph)
    {
      csfEntry *attributesEntry = file.CreateEntry("attributes");
      shaderGraphEntry->AddChild(attributesEntry);

      csfEntry *blendOutWithBinaryGradientEntry = file.CreateEntry("blendOutWithBinaryGradient");
      if (shaderGraph->IsBlendOutWithBinaryGradient())
      {
        blendOutWithBinaryGradientEntry->AddAttribute("enable", "true");
      }
      attributesEntry->AddChild(blendOutWithBinaryGradientEntry);

      csfEntry *discardAlphaEntry = file.CreateEntry("discardAlpha");
      if (shaderGraph->IsDiscardAlpha())
      {
        discardAlphaEntry->AddAttribute("enabled", "true");
      }
      attributesEntry->AddChild(discardAlphaEntry);


      csfEntry *thresholdEntry = file.CreateEntry("threshold");
      thresholdEntry->AddAttribute(std::to_string(shaderGraph->GetDiscardAlphaThreshold()));
      discardAlphaEntry->AddChild(thresholdEntry);

      std::string modeString;
      switch (shaderGraph->GetDiscardAlphaCompareMode())
      {
      case eCM_LessOrEqual: modeString = "LessOrEqual"; break;
      case eCM_GreaterOrEqual: modeString = "GreaterOrEqual"; break;
      case eCM_Less: modeString = "Less"; break;
      case eCM_Greater: modeString = "Greater"; break;
      case eCM_Equal: modeString = "Equal"; break;
      case eCM_NotEqual: modeString = "NotEqual"; break;
      case eCM_Never: modeString = "Never"; break;
      case eCM_Always: modeString = "Always"; break;
      }
      csfEntry *modeEntry = file.CreateEntry("mode");
      modeEntry->AddAttribute(modeString);
      discardAlphaEntry->AddChild(modeEntry);



      csfEntry *metaShaderGraphEntry = file.CreateEntry("shaderGraph");
      shaderGraphMetaEntry->AddChild(metaShaderGraphEntry);

      csfEntry *posEntry = file.CreateEntry("pos");
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().x()));
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().y()));
      metaShaderGraphEntry->AddChild(posEntry);

    }


  }

  for (size_t i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    csSGNode *node = m_shaderGraphCopy->GetNode(i);
    for (size_t j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      csfEntry *inputEntry = file.CreateEntry("node");
      inputEntry->AddAttribute("id", std::to_string(nodeMap[node]));
      inputEntry->AddAttribute("input", std::to_string(j));
      inputsEntry->AddChild(inputEntry);


      csSGInput *input = node->GetInput(j);
      csSGOutput *output = input->GetInput();
      if (output)
      {
        csSGNode *outputNode = output->GetNode();

        csfEntry *outputEntry = file.CreateEntry("node");
        outputEntry->AddAttribute("id", std::to_string(nodeMap[outputNode]));
        outputEntry->AddAttribute("output", std::to_string(output->GetIdx()));
        inputEntry->AddChild(outputEntry);
      }
      else
      {
        csfEntry *floatEntry = file.CreateEntry("float");
        floatEntry->AddAttribute(std::to_string(input->GetConst()));
        inputEntry->AddChild(floatEntry);
      }
    }
  }

  for (size_t i = 0; i < csSGShaderGraph::eIT_COUNT; ++i)
  {
    csSGOutput *output = m_shaderGraphCopy->GetInput((csSGShaderGraph::InputType)i);
    if (output)
    {
      csfEntry *sgEntry = file.CreateEntry("shaderGraph");
      switch (i)
      {
      case csSGShaderGraph::eIT_Diffuse:
        sgEntry->AddAttribute("input", "Diffuse");
        break;
      case csSGShaderGraph::eIT_Alpha:
        sgEntry->AddAttribute("input", "Alpha");
        break;
      case csSGShaderGraph::eIT_Roughness:
        sgEntry->AddAttribute("input", "Roughness");
        break;
      case csSGShaderGraph::eIT_Normal:
        sgEntry->AddAttribute("input", "Normal");
        break;
      }
      inputsEntry->AddChild(sgEntry);

      csSGNode *outputNode = output->GetNode();
      nodeId = nodeMap[outputNode];

      csfEntry *outputEntry = file.CreateEntry("node");
      outputEntry->AddAttribute("id", std::to_string(nodeId));
      outputEntry->AddAttribute("output", std::to_string(output->GetIdx()));
      sgEntry->AddChild(outputEntry);
    }
  }

  

  QString fileName = m_editor->GetResourceFileName();

  file.Output(std::string((const char*)fileName.toLatin1()));


  Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(m_editor->GetAssetDescriptor().GetLocator().GetResourceFile());

}

void ShaderGraphEditorWidget::on_pbScreenshot_clicked()
{
  m_editor->UpdatePreview();
}


bool ShaderGraphEditorWidget::Apply()
{

  printf("Apply\n");
  printf("Compile shader graph copy\n");
  csSGDefaultConsoleLogger logger;
  if (csEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraphCopy, &logger))
  {
    printf("Successfully compiled\n");
  }
  else
  {
    printf("Compiled with errors\n");
    return false;
  }

  m_shaderGraph = m_shaderGraphCopy->Copy(m_shaderGraph);
  printf("Compile shader graph\n");
  bool result = csEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraph);
  printf("Compile done: %d\n", result);

  emit ShaderGraphChanged();
  return true;
}

ShaderGraphEditorNode *ShaderGraphEditorWidget::GetEditorNode(csSGNode* node)
{
  for (size_t i = 0, in = m_gui.nodeGraph->GetNumberOfNodes(); i < in; ++i)
  {
    NodeGraphNode *nodeGraphNode = m_gui.nodeGraph->GetNode(i);
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(nodeGraphNode);
    if (editorNode->GetSGNode() == node)
    {
      return editorNode;
    }
  }
  return 0;
}

ShaderGraphEditorNode *ShaderGraphEditorWidget::GetShaderGraphNode()
{
  for (size_t i = 0, in = m_gui.nodeGraph->GetNumberOfNodes(); i < in; ++i)
  {
    NodeGraphNode *nodeGraphNode = m_gui.nodeGraph->GetNode(i);
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(nodeGraphNode);
    if (editorNode->GetSGNode() == 0 && editorNode->GetShaderGraph())
    {
      return editorNode;
    }
  }
  return 0;
}


std::string ShaderGraphEditorWidget::ExtractName(const csResourceLocator &locator)
{
  QString name(locator.GetResourceFile().c_str());
  name = name.replace("\\", "/");
  int idx_s = name.lastIndexOf("/");
  int idx_e = name.lastIndexOf(".");
  if (idx_e != -1)
  {
    name = name.mid(idx_s + 1, idx_e - idx_s - 1);
  }
  return std::string((const char*)name.toLatin1());
}
