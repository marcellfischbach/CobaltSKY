
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>
#include <shadergrapheditor/shadergrapheditortoolboxmodel.hh>

#include <nodegraph/nodegraphnode.hh>
#include <nodegraph/nodegraphnodeanchor.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>

#include <mimehelper.hh>

#include <valkyrie/vkengine.hh>
#include <valkyrie/core/vkclassregistry.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/shadergraph/vksgdefaultconsolelogger.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <valkyrie/graphics/shadergraph/vksgtexture2d.hh>

#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QDropEvent>
#include <QFile>

ShaderGraphEditorWidget::ShaderGraphEditorWidget(ShaderGraphEditor *editor)
  : QWidget()
  , m_editor(editor)
  , m_shaderGraph(0)
  , m_shaderGraphCopy(new vkSGShaderGraph())
  , m_updateGuard(false)
{
  m_gui.setupUi(this);
  on_nodeGraph_ScaleChanged(1.0f);

  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(shaderGraph);
  m_gui.nodeGraph->AddNode(shaderGraphNode);
}

ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  VK_RELEASE(m_shaderGraph);
  VK_RELEASE(m_shaderGraphCopy);
}

vkSGShaderGraph *ShaderGraphEditorWidget::SetShaderGraph(vkSGShaderGraph *shaderGraph, ShaderGraphEditorMeta *meta)
{
  VK_SET(m_shaderGraph, shaderGraph);
  if (!m_shaderGraph)
  {
    return 0;
  }

  m_updateGuard = true;
  m_shaderGraphCopy = m_shaderGraph->Copy(m_shaderGraphCopy);

  m_gui.nodeGraph->Clear();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(m_shaderGraphCopy);
  vkVector2f shaderGraphNodePos = meta->GetPos();
  shaderGraphNode->SetLocation(QPointF(shaderGraphNodePos.x, shaderGraphNodePos.y));
  m_gui.nodeGraph->AddNode(shaderGraphNode);

  std::map<vkSGNode *, ShaderGraphEditorNode*> mapping;
  for (vkSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *node = m_shaderGraphCopy->GetNode(i);

    ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(node);
    vkVector2f nodePos = meta->GetPos(i);
    editorNode->SetLocation(QPointF(nodePos.x, nodePos.y));
    m_gui.nodeGraph->AddNode(editorNode);

    mapping[node] = editorNode;
  }

  for (vkSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *node = m_shaderGraphCopy->GetNode(i);
    for (vkSize j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      vkSGInput* input = node->GetInput(j);
      vkSGOutput *output = input->GetInput();
      if (output)
      {
        vkSGNode *outputNode = output->GetNode();

        ShaderGraphEditorNode* editorNode = mapping[node];
        ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

        NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
        NodeGraphNodeAnchor *inputAnchor = editorNode->GetInputAnchor(input->GetIdx());

        m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
      }
    }
  }

  for (vkSize i = 0; i < vkSGShaderGraph::eIT_COUNT; ++i)
  {
    vkSGShaderGraph::InputType inputType = (vkSGShaderGraph::InputType)i;
    vkSGOutput *output = m_shaderGraphCopy->GetInput(inputType);
    if (output)
    {
      vkSGNode *outputNode = output->GetNode();
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
    vkSGNode *nodeOutput = editorNodeOutput->GetSGNode();
    vkSGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    vkSGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((vkSGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), output);
  }
  else
  {
    // make the connection between two nodes
    vkSGNode *nodeOutput = editorNodeOutput->GetSGNode();
    vkSGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    vkSGNode *nodeInput = editorNodeInput->GetSGNode();
    vkSGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());

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
    vkSGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((vkSGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), 0);
  }
  else
  {
    // make the connection between two nodes
    vkSGNode *nodeInput = editorNodeInput->GetSGNode();
    vkSGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());
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

    const vkClass *cls = MimeHelper::GetClass(data);
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
    const vkClass *cls = vkClassRegistry::Get()->GetClass(vkString((const char*)className.toLatin1()));
    if (!cls)
    {
      return;
    }

    vkSGNode *node = cls->CreateInstance<vkSGNode>();
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

    const vkClass *cls = MimeHelper::GetClass(data);
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
    vkResourceLocator locator = MimeHelper::GetResourceLocator(data);

    vkSGTexture2D *txtNode = new vkSGTexture2D();
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
  vkSGNode *sgNode = editorNode->GetSGNode();
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

  QDomDocument doc;

  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement shaderGraphElement = doc.createElement("shaderGraph");
  QDomElement metaElement = doc.createElement("meta");
  QDomElement shaderGraphMetaElement = doc.createElement("shaderGraphMeta");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  assetElement.appendChild(metaElement);
  dataElement.appendChild(shaderGraphElement);
  metaElement.appendChild(shaderGraphMetaElement);


  QDomElement nodesElement = doc.createElement("nodes");
  shaderGraphElement.appendChild(nodesElement);

  QDomElement inputsElement = doc.createElement("inputs");
  shaderGraphElement.appendChild(inputsElement);

  QDomElement metaNodesElement = doc.createElement("nodes");
  shaderGraphMetaElement.appendChild(metaNodesElement);

  size_t nodeId = 0;
  std::map<vkSGNode*, size_t> nodeMap;
  for (size_t i = 0, in = m_gui.nodeGraph->GetNumberOfNodes(); i < in; ++i)
  {
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(m_gui.nodeGraph->GetNode(i));
    vkSGNode *node = editorNode->GetSGNode();
    if (node)
    {

      nodeMap[node] = nodeId;

      QDomElement nodeElement = doc.createElement("node");
      nodeElement.setAttribute("id", nodeId);
      nodeElement.setAttribute("class", QString(node->GetClass()->GetName().c_str()));
      nodesElement.appendChild(nodeElement);

      vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(node);
      if (resourceNode)
      {
        QDomElement resourceElement = doc.createElement("resource");
        resourceElement.setAttribute("name", QString(resourceNode->GetResourceName().c_str()));
        nodeElement.appendChild(resourceElement);

        switch (resourceNode->GetResourceType())
        {
        case eSPT_Float:
          resourceElement.appendChild(doc.createElement("float"))
            .appendChild(doc.createTextNode(QString("%1")
              .arg((double)resourceNode->GetDefaultFloats()[0], 0, 'f')));
          break;
        case eSPT_Vector2:
          resourceElement.appendChild(doc.createElement("float2"))
            .appendChild(doc.createTextNode(QString("%1, %2")
              .arg((double)resourceNode->GetDefaultFloats()[0], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[1], 0, 'f')
            ));
          break;
        case eSPT_Vector3:
          resourceElement.appendChild(doc.createElement("float3"))
            .appendChild(doc.createTextNode(QString("%1, %2, %3")
              .arg((double)resourceNode->GetDefaultFloats()[0], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[1], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[2], 0, 'f')
            ));
          break;
        case eSPT_Vector4:
          resourceElement.appendChild(doc.createElement("float4"))
            .appendChild(doc.createTextNode(QString("%1, %2, %3, %4")
              .arg((double)resourceNode->GetDefaultFloats()[0], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[1], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[2], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[3], 0, 'f')
            ));
          break;
        case eSPT_Int:
          resourceElement.appendChild(doc.createElement("int"))
            .appendChild(doc.createTextNode(QString("%1")
              .arg(resourceNode->GetDefaultInts()[0])
            ));
          break;
        case eSPT_IVector2:
          resourceElement.appendChild(doc.createElement("int2"))
            .appendChild(doc.createTextNode(QString("%1, %2")
              .arg(resourceNode->GetDefaultInts()[0])
              .arg(resourceNode->GetDefaultInts()[1])
            ));
          break;
        case eSPT_IVector3:
          resourceElement.appendChild(doc.createElement("int3"))
            .appendChild(doc.createTextNode(QString("%1, %2, %3")
              .arg(resourceNode->GetDefaultInts()[0])
              .arg(resourceNode->GetDefaultInts()[1])
              .arg(resourceNode->GetDefaultInts()[2])
            ));
          break;
        case eSPT_IVector4:
          resourceElement.appendChild(doc.createElement("int4"))
            .appendChild(doc.createTextNode(QString("%1, %2, %3, %4")
              .arg(resourceNode->GetDefaultInts()[0])
              .arg(resourceNode->GetDefaultInts()[1])
              .arg(resourceNode->GetDefaultInts()[2])
              .arg(resourceNode->GetDefaultInts()[3])
            ));
          break;
        case eSPT_Color4:
          resourceElement.appendChild(doc.createElement("color4"))
            .appendChild(doc.createTextNode(QString("%1, %2, %3, %4")
              .arg((double)resourceNode->GetDefaultFloats()[0], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[1], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[2], 0, 'f')
              .arg((double)resourceNode->GetDefaultFloats()[3], 0, 'f')
            ));
          break;
        case eSPT_Matrix3:
          break;
        case eSPT_Matrix4:
          break;
        case eSPT_Texture:
          resourceElement.appendChild(doc.createElement("locator"))
            .appendChild(doc.createTextNode(QString(
              resourceNode->GetDefaultTextureResource().GetResourceFile().c_str()
            )));
          break;
        }
      }
      nodeElement = doc.createElement("node");
      nodeElement.setAttribute("id", nodeId);
      metaNodesElement.appendChild(nodeElement);

      nodeElement.appendChild(doc.createElement("pos"))
        .appendChild(doc.createTextNode(QString("%1, %2")
          .arg(editorNode->GetLocation().x(), 0, 'f')
          .arg(editorNode->GetLocation().y(), 0, 'f')));

      nodeId++;
    }
    vkSGShaderGraph *shaderGraph = editorNode->GetShaderGraph();
    if (shaderGraph)
    {
      QDomElement attributesElement = doc.createElement("attributes");
      shaderGraphElement.appendChild(attributesElement);

      QDomElement blendOutWithBinaryGradientElement = doc.createElement("blendOutWithBinaryGradient");
      if (shaderGraph->IsBlendOutWithBinaryGradient())
      {
        blendOutWithBinaryGradientElement.setAttribute("enabled", "true");
      }
      attributesElement.appendChild(blendOutWithBinaryGradientElement);

      QDomElement discardAlphaElement = doc.createElement("discardAlpha");
      if (shaderGraph->IsDiscardAlpha())
      {
        discardAlphaElement.setAttribute("enabled", "true");
      }
      attributesElement.appendChild(discardAlphaElement);


      discardAlphaElement.appendChild(doc.createElement("threshold")).
        appendChild(doc.createTextNode(QString("%1").arg((double)shaderGraph->GetDiscardAlphaThreshold(), 0, 'f')));

      QString modeString;
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
      discardAlphaElement.appendChild(doc.createElement("mode"))
        .appendChild(doc.createTextNode(modeString));



      QDomElement metaShaderGraphElement = doc.createElement("shaderGraph");
      shaderGraphMetaElement.appendChild(metaShaderGraphElement);
      metaShaderGraphElement.appendChild(doc.createElement("pos"))
        .appendChild(doc.createTextNode(QString("%1, %2")
          .arg(editorNode->GetLocation().x(), 0, 'f')
          .arg(editorNode->GetLocation().y(), 0, 'f')));


    }


  }

  for (size_t i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *node = m_shaderGraphCopy->GetNode(i);
    for (size_t j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      QDomElement inputElement = doc.createElement("node");
      inputElement.setAttribute("id", nodeMap[node]);
      inputElement.setAttribute("input", j);
      inputsElement.appendChild(inputElement);

      vkSGInput *input = node->GetInput(j);
      vkSGOutput *output = input->GetInput();
      if (output)
      {
        vkSGNode *outputNode = output->GetNode();

        QDomElement outputElement = doc.createElement("node");
        outputElement.setAttribute("id", nodeMap[outputNode]);
        outputElement.setAttribute("output", output->GetIdx());
        inputElement.appendChild(outputElement);
      }
      else
      {
        QDomElement floatElement = doc.createElement("float");
        floatElement.appendChild(doc.createTextNode(QString("%1").arg((double)input->GetConst(), 0, 'f')));
        inputElement.appendChild(floatElement);
      }
    }
  }

  for (size_t i = 0; i < vkSGShaderGraph::eIT_COUNT; ++i)
  {
    vkSGOutput *output = m_shaderGraphCopy->GetInput((vkSGShaderGraph::InputType)i);
    if (output)
    {
      QDomElement sgElement = doc.createElement("shaderGraph");
      switch (i)
      {
      case vkSGShaderGraph::eIT_Diffuse:
        sgElement.setAttribute("input", "Diffuse");
        break;
      case vkSGShaderGraph::eIT_Alpha:
        sgElement.setAttribute("input", "Alpha");
        break;
      case vkSGShaderGraph::eIT_Roughness:
        sgElement.setAttribute("input", "Roughness");
        break;
      case vkSGShaderGraph::eIT_Normal:
        sgElement.setAttribute("input", "Normal");
        break;
      }
      inputsElement.appendChild(sgElement);

      vkSGNode *outputNode = output->GetNode();
      nodeId = nodeMap[outputNode];

      QDomElement outputElement = doc.createElement("node");
      outputElement.setAttribute("id", nodeId);
      outputElement.setAttribute("output", output->GetIdx());
      sgElement.appendChild(outputElement);
    }
  }

  /* Old handling of inputs
  for (size_t i = 0, in = m_gui.nodeGraph->GetNumberOfConnections(); i < in; ++i)
  {
    const NodeGraphWidget::Conn conn = m_gui.nodeGraph->GetConnection(i);
    ShaderGraphEditorNode *inputNode = static_cast<ShaderGraphEditorNode*>(conn.inputNode);
    ShaderGraphEditorNode *outputNode = static_cast<ShaderGraphEditorNode*>(conn.outputNode);

    if (!inputNode || !outputNode)
    {
      continue;
    }
    QDomElement inputElement;
    if (inputNode->GetShaderGraph())
    {
      inputElement = doc.createElement("shaderGraph");
      switch (conn.inputNodeInputIdx)
      {
      case vkSGShaderGraph::eIT_Diffuse:
        inputElement.setAttribute("input", "Diffuse");
        break;
      case vkSGShaderGraph::eIT_Alpha:
        inputElement.setAttribute("input", "Alpha");
        break;
      case vkSGShaderGraph::eIT_Roughness:
        inputElement.setAttribute("input", "Roughness");
        break;
      case vkSGShaderGraph::eIT_Normal:
        inputElement.setAttribute("input", "Normal");
        break;
      }
    }
    else if (inputNode->GetSGNode())
    {
      inputElement = doc.createElement("node");
      inputElement.setAttribute("id", nodeMap[inputNode]);
      inputElement.setAttribute("input", conn.inputNodeInputIdx);
    }
    else
    {
      continue;
    }
    if (!outputNode->GetSGNode())
    {
      continue;
    }
    inputsElement.appendChild(inputElement);

    QDomElement outputElement = doc.createElement("node");
    outputElement.setAttribute("id", nodeMap[outputNode]);
    outputElement.setAttribute("output", conn.outputNodeOutputIdx);
    inputElement.appendChild(outputElement);
  }
  */



  printf("XML:\n%s\n", (const char*)doc.toString(2).toLatin1());
  printf("FileName: %s\n", m_editor->GetAssetDescriptor().GetAssetFileName().c_str());
  QFile file(QString(m_editor->GetAssetDescriptor().GetAssetFileName().c_str()));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(doc.toString(2).toLatin1());
    file.close();
  }
}


bool ShaderGraphEditorWidget::Apply()
{

  printf("Apply\n");
  printf("Compile shader graph copy\n");
  vkSGDefaultConsoleLogger logger;
  if (vkEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraphCopy, &logger))
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
  bool result = vkEng->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(m_shaderGraph);
  printf("Compile done: %d\n", result);

  emit ShaderGraphChanged();
  return true;
}

ShaderGraphEditorNode *ShaderGraphEditorWidget::GetEditorNode(vkSGNode* node)
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


vkString ShaderGraphEditorWidget::ExtractName(const vkResourceLocator &locator)
{
  QString name(locator.GetResourceFile().c_str());
  name = name.replace("\\", "/");
  int idx_s = name.lastIndexOf("/");
  int idx_e = name.lastIndexOf(".");
  if (idx_e != -1)
  {
    name = name.mid(idx_s + 1, idx_e - idx_s - 1);
  }
  return vkString((const char*)name.toLatin1());
}