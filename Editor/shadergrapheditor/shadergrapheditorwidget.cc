
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>
#include <shadergrapheditor/shadergrapheditornode.hh>
#include <shadergrapheditor/shadergrapheditortoolboxmodel.hh>

#include <editor/components/nodegraph/nodegraphnode.hh>
#include <editor/components/nodegraph/nodegraphnodeanchor.hh>
#include <editor/components/nodegraph/nodegraphnodeheader.hh>
#include <editor/components/nodegraph/nodegraphnodeimageproperty.hh>
#include <editor/components/nodegraph/nodegraphnodevalueproperty.hh>

#include <materialeditor/materialeditorevents.hh>

#include <editor/mimehelper.hh>
#include <editor/editor.hh>
#include <editor/eventbus.hh>
#include <editor/project/project.hh>

#include <cobalt/csengine.hh>
#include <csrefl/classregistry.hh>
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
#include <QUuid>


ShaderGraphEditorWidget::ShaderGraphEditorWidget(ShaderGraphEditor *editor)
  : QWidget()
  , m_editor(editor)
  , m_shaderGraph(0)
  , m_shaderGraphCopy(new cs::SGShaderGraph())
  , m_updateGuard(false)
{
  m_gui.setupUi(this);
  on_nodeGraph_ScaleChanged(1.0f);

  cs::SGShaderGraph *shaderGraph = new cs::SGShaderGraph();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(shaderGraph);
  m_gui.nodeGraph->AddNode(shaderGraphNode);
}

ShaderGraphEditorWidget::~ShaderGraphEditorWidget()
{
  CS_RELEASE(m_shaderGraph);
  CS_RELEASE(m_shaderGraphCopy);
}

cs::SGShaderGraph *ShaderGraphEditorWidget::SetShaderGraph(cs::SGShaderGraph *shaderGraph, ShaderGraphEditorMeta *meta)
{
  CS_SET(m_shaderGraph, shaderGraph);
  if (!m_shaderGraph)
  {
    return 0;
  }

  m_updateGuard = true;
  m_shaderGraphCopy = m_shaderGraph->Copy(m_shaderGraphCopy);
  QueryResources(m_shaderGraph);

  m_gui.nodeGraph->Clear();
  ShaderGraphEditorNode *shaderGraphNode = new ShaderGraphEditorNode(m_shaderGraphCopy);
  cs::Vector2f shaderGraphNodePos = meta->GetPos();
  shaderGraphNode->SetLocation(QPointF(shaderGraphNodePos.x, shaderGraphNodePos.y));
  m_gui.nodeGraph->AddNode(shaderGraphNode);

  std::map<cs::SGNode *, ShaderGraphEditorNode*> mapping;
  for (csSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    cs::SGNode *node = m_shaderGraphCopy->GetNode(i);

    ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(node);
    cs::Vector2f nodePos = meta->GetPos(i);
    editorNode->SetLocation(QPointF(nodePos.x, nodePos.y));
    m_gui.nodeGraph->AddNode(editorNode);

    mapping[node] = editorNode;
  }

  for (csSize i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    cs::SGNode *node = m_shaderGraphCopy->GetNode(i);
    for (csSize j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      cs::SGInput* input = node->GetInput(j);
      cs::SGOutput *output = input->GetInput();
      if (output)
      {
        cs::SGNode *outputNode = output->GetNode();

        ShaderGraphEditorNode* editorNode = mapping[node];
        ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

        NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
        NodeGraphNodeAnchor *inputAnchor = editorNode->GetInputAnchor(input->GetIdx());

        m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
      }
    }
  }

  for (csSize i = 0; i < cs::SGShaderGraph::eIT_COUNT; ++i)
  {
    cs::SGShaderGraph::InputType inputType = (cs::SGShaderGraph::InputType)i;
    cs::SGOutput *output = m_shaderGraphCopy->GetInput(inputType);
    if (output)
    {
      cs::SGNode *outputNode = output->GetNode();
      ShaderGraphEditorNode* editorOutputNode = mapping[outputNode];

      NodeGraphNodeAnchor *outputAnchor = editorOutputNode->GetOutputAnchor(output->GetIdx());
      NodeGraphNodeAnchor *inputAnchor = shaderGraphNode->GetInputAnchor(i);

      m_gui.nodeGraph->Connect(outputAnchor, inputAnchor);
    }

  }
  m_updateGuard = false;
  return m_shaderGraphCopy;
}


void ShaderGraphEditorWidget::QueryResources(cs::SGShaderGraph *graph)
{
  m_resourceIDs.clear();
  for (csSize i = 0, in = graph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    cs::SGNode *node = graph->GetNode(i);
    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (resourceNode)
    {
      m_resourceIDs.insert(resourceNode->GetResourceId());
    }
  }
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
    cs::SGNode *nodeOutput = editorNodeOutput->GetSGNode();
    cs::SGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    cs::SGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((cs::SGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), output);
  }
  else
  {
    // make the connection between two nodes
    cs::SGNode *nodeOutput = editorNodeOutput->GetSGNode();
    cs::SGOutput *output = nodeOutput->GetOutput(outputAnchor->GetProperty()->GetIdx());

    cs::SGNode *nodeInput = editorNodeInput->GetSGNode();
    cs::SGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());

    input->SetInput(output);
  }
}

void ShaderGraphEditorWidget::on_nodeGraph_Disconnected(NodeGraphNodeAnchor *anchorA, NodeGraphNodeAnchor *anchorB)
{
  if (m_updateGuard)
  {
    return;
  }

  NodeGraphNodeAnchor *inputAnchor = anchorA->GetType() == eNGNPT_Input ? anchorA : anchorB;

  ShaderGraphEditorNode *editorNodeInput = static_cast<ShaderGraphEditorNode*>(inputAnchor->GetNode());

  if (editorNodeInput->GetShaderGraph())
  {
    // make the connection to the shader graph
    cs::SGShaderGraph *shaderGraph = editorNodeInput->GetShaderGraph();
    shaderGraph->SetInput((cs::SGShaderGraph::InputType)inputAnchor->GetProperty()->GetIdx(), 0);
  }
  else
  {
    // make the connection between two nodes
    cs::SGNode *nodeInput = editorNodeInput->GetSGNode();
    cs::SGInput *input = nodeInput->GetInput(inputAnchor->GetProperty()->GetIdx());
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
  bool validEntryFound = false;
  if (event->mimeData()->hasFormat(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME))
  {
    validEntryFound = true;
  }
  else
  {
    const QMimeData *data = event->mimeData();
    if (!data->hasFormat("application/assetModelEntryPtr"))
    {
      return;
    }

    QByteArray rawData = data->data("application/assetModelEntryPtr");
    QDataStream entriesStream(&rawData, QIODevice::ReadOnly);
    std::vector<asset::model::Entry*> entries;
    get(entriesStream, entries);

    for (asset::model::Entry *entry : entries)
    {
      asset::model::Asset *asset = entry->AsAsset();
      if (!asset)
      {
        continue;
      }

      const cs::Class *cls = asset->GetClass();
      if (!cls)
      {
        continue;
      }

      if (cls->IsInstanceOf(cs::Texture2DWrapper::GetStaticClass()))
      {
        validEntryFound = true;
        break;
      }
    }
  }

  if (validEntryFound)
  {
    accept->Accept();
  }
}


void ShaderGraphEditorWidget::on_nodeGraph_DragDropped(const QDropEvent *event)
{

  if (event->mimeData()->hasFormat(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME))
  {
    QByteArray ba = event->mimeData()->data(SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME);
    QDataStream stream(&ba, QIODevice::ReadOnly);
    QString className;
    stream >> className;
    const cs::Class *cls = cs::ClassRegistry::Get()->GetClass(std::string((const char*)className.toLatin1()));
    if (!cls)
    {
      return;
    }

    cs::SGNode *node = cls->CreateInstance<cs::SGNode>();
    if (!node)
    {
      return;
    }

    cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (resourceNode)
    {
      QString uuid = QUuid::createUuid().toString();
      resourceNode->SetResourceId(std::string(uuid.toLatin1().data()));
    }

    emit ShaderGraphNodeAboutToAdd(node);
    m_shaderGraphCopy->AddNode(node);
    emit ShaderGraphNodeAdded(node);

    ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(node);
    m_gui.nodeGraph->AddNode(editorNode);
    editorNode->SetLocation(m_gui.nodeGraph->GetLocalCoordinate(event->pos()));

  }
  else
  {


    const QMimeData *data = event->mimeData();
    if (!data)
    {
      return;
    }



    if (!data->hasFormat("application/assetModelEntryPtr"))
    {
      return;
    }

    QByteArray rawData = data->data("application/assetModelEntryPtr");
    QDataStream entriesStream(&rawData, QIODevice::ReadOnly);
    std::vector<asset::model::Entry*> entries;
    get(entriesStream, entries);

    for (asset::model::Entry *entry : entries)
    {
      asset::model::Asset *asset = entry->AsAsset();
      if (!asset)
      {
        return;
      }

      const cs::Class *cls = asset->GetClass();
      if (!cls)
      {
        continue;
      }

      if (!cls->IsInstanceOf(cs::Texture2DWrapper::GetStaticClass()))
      {
        continue;
      }


      cs::ResourceLocator locator = asset->GetResourceLocator();

      cs::SGTexture2D *txtNode = new cs::SGTexture2D();

      txtNode->SetDefaultTextureResource(locator);
      txtNode->SetResourceName(ExtractName(locator));
      QString uuid = QUuid::createUuid().toString();
      txtNode->SetResourceId(std::string(uuid.toLatin1().data()));
      emit ShaderGraphNodeAboutToAdd(txtNode);
      m_shaderGraphCopy->AddNode(txtNode);
      emit ShaderGraphNodeAdded(txtNode);

      ShaderGraphEditorNode *editorNode = new ShaderGraphEditorNode(txtNode);

      m_gui.nodeGraph->AddNode(editorNode);
      editorNode->SetLocation(m_gui.nodeGraph->GetLocalCoordinate(event->pos()));

    }

  }

  m_gui.nodeGraph->repaint();
}

void ShaderGraphEditorWidget::get(QDataStream &stream, std::vector<asset::model::Entry*> &entries) const
{
  quint32 numEntries;
  stream >> numEntries;
  for (quint32 i = 0; i < numEntries; ++i)
  {
    quint64 ptr;
    stream >> ptr;
    asset::model::Entry *entry = reinterpret_cast<asset::model::Entry*>(ptr);
    entries.push_back(entry);
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
  ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(node);

  cs::SGNode *sgNode = editorNode->GetSGNode();
  if (sgNode)
  {
    emit ShaderGraphNodeAboutToRemove(sgNode);
    m_shaderGraphCopy->RemoveNode(sgNode);
    emit ShaderGraphNodeRemoved(sgNode);
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
  cs::file::File file;

  cs::file::Entry *assetEntry = file.CreateEntry("asset");
  cs::file::Entry *dataEntry = file.CreateEntry("data");
  cs::file::Entry *shaderGraphEntry = file.CreateEntry("shaderGraph");
  cs::file::Entry *metaEntry = file.CreateEntry("meta");
  cs::file::Entry *shaderGraphMetaEntry = file.CreateEntry("shaderGraphMeta");

  file.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  assetEntry->AddChild(metaEntry);
  dataEntry->AddChild(shaderGraphEntry);
  metaEntry->AddChild(shaderGraphMetaEntry);


  cs::file::Entry *nodesEntry = file.CreateEntry("nodes");
  shaderGraphEntry->AddChild(nodesEntry);

  cs::file::Entry *inputsEntry = file.CreateEntry("inputs");
  shaderGraphEntry->AddChild(inputsEntry);

  cs::file::Entry *metaNodesEntry = file.CreateEntry("nodes");
  shaderGraphMetaEntry->AddChild(metaNodesEntry);

  size_t nodeId = 0;
  std::map<cs::SGNode*, size_t> nodeMap;
  for (size_t i = 0, in = m_gui.nodeGraph->GetNumberOfNodes(); i < in; ++i)
  {
    ShaderGraphEditorNode *editorNode = static_cast<ShaderGraphEditorNode*>(m_gui.nodeGraph->GetNode(i));
    cs::SGNode *node = editorNode->GetSGNode();
    if (node)
    {

      nodeMap[node] = nodeId;

      cs::file::Entry *nodeEntry = file.CreateEntry("node");
      nodeEntry->AddAttribute("id", std::to_string(nodeId));
      nodeEntry->AddAttribute("class", node->GetClass()->GetName());
      nodesEntry->AddChild(nodeEntry);

      cs::SGResourceNode *resourceNode = cs::QueryClass<cs::SGResourceNode>(node);
      if (resourceNode)
      {
        cs::file::Entry *resourceEntry = file.CreateEntry("resource");
        resourceEntry->AddAttribute("id", resourceNode->GetResourceId());
        resourceEntry->AddAttribute("name", resourceNode->GetResourceName());
        nodeEntry->AddChild(resourceEntry);

        cs::file::Entry *valueEntry = 0;
        switch (resourceNode->GetResourceType())
        {
        case cs::eSPT_Float:
          valueEntry = file.CreateEntry("float");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          break;
        case cs::eSPT_Vector2:
          valueEntry = file.CreateEntry("float2");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          break;
        case cs::eSPT_Vector3:
          valueEntry = file.CreateEntry("float3");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[2]));
          break;
        case cs::eSPT_Vector4:
          valueEntry = file.CreateEntry("float4");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[2]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[3]));
          break;
        case cs::eSPT_Int:
          valueEntry = file.CreateEntry("int");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          break;
        case cs::eSPT_IVector2:
          valueEntry = file.CreateEntry("int2");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[1]));
          break;
        case cs::eSPT_IVector3:
          valueEntry = file.CreateEntry("int3");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[2]));
          break;
        case cs::eSPT_IVector4:
          valueEntry = file.CreateEntry("int4");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[2]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultInts()[3]));
          break;
        case cs::eSPT_Color4:
          valueEntry = file.CreateEntry("color4");
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[0]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[1]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[2]));
          valueEntry->AddAttribute(std::to_string(resourceNode->GetDefaultFloats()[3]));
          break;
        case cs::eSPT_Matrix3:
          break;
        case cs::eSPT_Matrix4:
          break;
        case cs::eSPT_Texture:
          valueEntry = file.CreateEntry("locator");
          valueEntry->AddAttribute("locator", resourceNode->GetDefaultTextureResource().GetResourceFile());
          break;
        }
        if (valueEntry)
        {
          resourceEntry->AddChild(valueEntry);
        }
      }
      cs::file::Entry *nodeEntry1 = file.CreateEntry("node");
      nodeEntry1->AddAttribute("id", std::to_string(nodeId));
      metaNodesEntry->AddChild(nodeEntry1);

      cs::file::Entry *posEntry = file.CreateEntry("pos");
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().x()));
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().y()));
      nodeEntry1->AddChild(posEntry);

      nodeId++;
    }
    cs::SGShaderGraph *shaderGraph = editorNode->GetShaderGraph();
    if (shaderGraph)
    {
      cs::file::Entry *attributesEntry = file.CreateEntry("attributes");
      shaderGraphEntry->AddChild(attributesEntry);

      cs::file::Entry *blendOutWithBinaryGradientEntry = file.CreateEntry("blendOutWithBinaryGradient");
      if (shaderGraph->IsBlendOutWithBinaryGradient())
      {
        blendOutWithBinaryGradientEntry->AddAttribute("enable", "true");
      }
      attributesEntry->AddChild(blendOutWithBinaryGradientEntry);

      cs::file::Entry *discardAlphaEntry = file.CreateEntry("discardAlpha");
      if (shaderGraph->IsDiscardAlpha())
      {
        discardAlphaEntry->AddAttribute("enabled", "true");
      }
      attributesEntry->AddChild(discardAlphaEntry);


      cs::file::Entry *thresholdEntry = file.CreateEntry("threshold");
      thresholdEntry->AddAttribute(std::to_string(shaderGraph->GetDiscardAlphaThreshold()));
      discardAlphaEntry->AddChild(thresholdEntry);

      std::string modeString;
      switch (shaderGraph->GetDiscardAlphaCompareMode())
      {
      case cs::eCM_LessOrEqual: modeString = "LessOrEqual"; break;
      case cs::eCM_GreaterOrEqual: modeString = "GreaterOrEqual"; break;
      case cs::eCM_Less: modeString = "Less"; break;
      case cs::eCM_Greater: modeString = "Greater"; break;
      case cs::eCM_Equal: modeString = "Equal"; break;
      case cs::eCM_NotEqual: modeString = "NotEqual"; break;
      case cs::eCM_Never: modeString = "Never"; break;
      case cs::eCM_Always: modeString = "Always"; break;
      }
      cs::file::Entry *modeEntry = file.CreateEntry("mode");
      modeEntry->AddAttribute(modeString);
      discardAlphaEntry->AddChild(modeEntry);



      cs::file::Entry *metaShaderGraphEntry = file.CreateEntry("shaderGraph");
      shaderGraphMetaEntry->AddChild(metaShaderGraphEntry);

      cs::file::Entry *posEntry = file.CreateEntry("pos");
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().x()));
      posEntry->AddAttribute(std::to_string(editorNode->GetLocation().y()));
      metaShaderGraphEntry->AddChild(posEntry);

    }

  }

  for (size_t i = 0, in = m_shaderGraphCopy->GetNumberOfTotalNodes(); i < in; ++i)
  {
    cs::SGNode *node = m_shaderGraphCopy->GetNode(i);
    for (size_t j = 0, jn = node->GetNumberOfInputs(); j < jn; ++j)
    {
      cs::file::Entry *inputEntry = file.CreateEntry("node");
      inputEntry->AddAttribute("id", std::to_string(nodeMap[node]));
      inputEntry->AddAttribute("input", std::to_string(j));
      inputsEntry->AddChild(inputEntry);


      cs::SGInput *input = node->GetInput(j);
      cs::SGOutput *output = input->GetInput();
      if (output)
      {
        cs::SGNode *outputNode = output->GetNode();

        cs::file::Entry *outputEntry = file.CreateEntry("node");
        outputEntry->AddAttribute("id", std::to_string(nodeMap[outputNode]));
        outputEntry->AddAttribute("output", std::to_string(output->GetIdx()));
        inputEntry->AddChild(outputEntry);
      }
      else
      {
        cs::file::Entry *floatEntry = file.CreateEntry("float");
        floatEntry->AddAttribute(std::to_string(input->GetConst()));
        inputEntry->AddChild(floatEntry);
      }
    }
  }

  for (size_t i = 0; i < cs::SGShaderGraph::eIT_COUNT; ++i)
  {
    cs::SGOutput *output = m_shaderGraphCopy->GetInput((cs::SGShaderGraph::InputType)i);
    if (output)
    {
      cs::file::Entry *sgEntry = file.CreateEntry("shaderGraph");
      switch (i)
      {
      case cs::SGShaderGraph::eIT_Diffuse:
        sgEntry->AddAttribute("input", "Diffuse");
        break;
      case cs::SGShaderGraph::eIT_Alpha:
        sgEntry->AddAttribute("input", "Alpha");
        break;
      case cs::SGShaderGraph::eIT_Roughness:
        sgEntry->AddAttribute("input", "Roughness");
        break;
      case cs::SGShaderGraph::eIT_Normal:
        sgEntry->AddAttribute("input", "Normal");
        break;
      }
      inputsEntry->AddChild(sgEntry);

      cs::SGNode *outputNode = output->GetNode();
      nodeId = nodeMap[outputNode];

      cs::file::Entry *outputEntry = file.CreateEntry("node");
      outputEntry->AddAttribute("id", std::to_string(nodeId));
      outputEntry->AddAttribute("output", std::to_string(output->GetIdx()));
      sgEntry->AddChild(outputEntry);
    }
  }



  QString fileName = m_editor->GetResourceFileName();

  file.Output(std::string((const char*)fileName.toLatin1()));

  // Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(m_editor->GetAsset()->GetResourceLocator().GetResourceFile());

}

void ShaderGraphEditorWidget::on_pbScreenshot_clicked()
{
  m_editor->UpdatePreview();
}


bool ShaderGraphEditorWidget::Apply()
{

  printf("Apply\n");
  printf("Compile shader graph copy\n");
  cs::SGDefaultConsoleLogger logger;

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

  std::set<std::string> thisResources;
  cs::ResourceLocator locator = m_editor->GetAsset()->GetResourceLocator();
  if (locator.IsValid())
  {
    for (csSize i = 0, in = m_shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
    {
      cs::SGNode *node = m_shaderGraph->GetNode(i);
      cs::SGResourceNode *resNode = cs::QueryClass<cs::SGResourceNode>(node);
      if (resNode)
      {
        thisResources.insert(resNode->GetResourceId());
        if (m_resourceIDs.find(resNode->GetResourceId()) == m_resourceIDs.end())
        {
            MaterialEditorAttributeAdded added(locator, resNode->GetResourceId(), resNode->GetResourceName());
          EventBus::Get().Fire(added);
        }
        else
        {
            MaterialEditorAttributeChanged changed(locator, resNode->GetResourceId(), resNode->GetResourceName());
          EventBus::Get().Fire(changed);
        }
      }
    }

    for (const std::string &id : m_resourceIDs)
    {
      if (thisResources.find(id) == thisResources.end())
      {
          MaterialEditorAttributeRemoved removed(locator, id);
        EventBus::Get().Fire(removed);
      }
    }

  }



  emit ShaderGraphChanged();
  return true;
}

ShaderGraphEditorNode *ShaderGraphEditorWidget::GetEditorNode(cs::SGNode* node)
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


std::string ShaderGraphEditorWidget::ExtractName(const cs::ResourceLocator &locator)
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
