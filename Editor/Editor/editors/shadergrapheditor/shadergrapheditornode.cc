

#include <editors/shadergrapheditor/shadergrapheditornode.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>
#include <editorimage.hh>
#include <QMap>
#include <QColor>


static const int IDX_TEXTURE_IMAGE = -1;


ShaderGraphEditorNode::ShaderGraphEditorNode(csSGShaderGraph *shaderGraph)
  : NodeGraphNode()
  , m_shaderGraph(0)
  , m_sgNode(0)
{
  CS_SET(m_shaderGraph, shaderGraph);
  GetHeader()->SetName("Shader");
  GetHeader()->SetColor0(QColor(128, 128, 128));
  GetHeader()->SetColor1(QColor(0, 0, 0));


  NodeGraphNodeValueProperty *propDiffuse = new NodeGraphNodeValueProperty(this);
  propDiffuse->SetAnchorShow(true);
  propDiffuse->SetShowValue(false);
  propDiffuse->SetName("Diffuse");
  propDiffuse->SetIdx(csSGShaderGraph::eIT_Diffuse);
  propDiffuse->Initialize();
  m_inputAnchors[csSGShaderGraph::eIT_Diffuse] = propDiffuse->GetAnchor();

  NodeGraphNodeValueProperty *propAlpha = new NodeGraphNodeValueProperty(this);
  propAlpha->SetAnchorShow(true);
  propAlpha->SetShowValue(false);
  propAlpha->SetName("Alpha");
  propAlpha->SetIdx(csSGShaderGraph::eIT_Alpha);
  propAlpha->Initialize();
  m_inputAnchors[csSGShaderGraph::eIT_Alpha] = propAlpha->GetAnchor();

  NodeGraphNodeValueProperty *propRoughness = new NodeGraphNodeValueProperty(this);
  propRoughness->SetAnchorShow(true);
  propRoughness->SetShowValue(false);
  propRoughness->SetName("Roughness");
  propRoughness->SetIdx(csSGShaderGraph::eIT_Roughness);
  propRoughness->Initialize();
  m_inputAnchors[csSGShaderGraph::eIT_Roughness] = propRoughness->GetAnchor();

  NodeGraphNodeValueProperty *propNormal = new NodeGraphNodeValueProperty(this);
  propNormal->SetAnchorShow(true);
  propNormal->SetShowValue(false);
  propNormal->SetName("Normal");
  propNormal->SetIdx(csSGShaderGraph::eIT_Normal);
  propNormal->Initialize();
  m_inputAnchors[csSGShaderGraph::eIT_Normal] = propNormal->GetAnchor();


  AddInputProperty(propDiffuse);
  AddInputProperty(propAlpha);
  AddInputProperty(propRoughness);
  AddInputProperty(propNormal);
  Layout();
}

ShaderGraphEditorNode::ShaderGraphEditorNode(csSGNode *node)
  : NodeGraphNode()
  , m_shaderGraph(0)
  , m_sgNode(0)
{
  CS_SET(m_sgNode, node);
  QString name(node->GetName().c_str());
  int idx = name.indexOf("/");
  if (idx != -1)
  {
    QString cat = name.left(idx);
    QMap<QString, QColor> mapColor0;
    mapColor0.insert("Math", QColor(255, 0, 0));
    mapColor0.insert("Assemble", QColor(0, 128, 0));
    mapColor0.insert("Var", QColor(0, 0, 255));
    mapColor0.insert("Const", QColor(128, 128, 0));
    mapColor0.insert("Texture", QColor(0, 128, 128));

    QMap<QString, QColor> mapColor1;
    mapColor1.insert("Math", QColor(0, 0, 0));
    mapColor1.insert("Assemble", QColor(0, 0, 0));
    mapColor1.insert("Var", QColor(0, 0, 0));
    mapColor1.insert("Const", QColor(0, 0, 0));
    mapColor1.insert("Texture", QColor(0, 0, 0));

    GetHeader()->SetColor0(mapColor0.value(cat, QColor(0, 0, 0)));
    GetHeader()->SetColor1(mapColor1.value(cat, QColor(0, 0, 0)));
  }

  GetHeader()->SetName(QString(node->GetName().c_str()));
  csSGResourceNode *resource = csQueryClass<csSGResourceNode>(node);
  if (resource)
  {
    NodeGraphNodeImageProperty *prop = new NodeGraphNodeImageProperty(this);

    prop->SetIdx(IDX_TEXTURE_IMAGE);
    AddInputProperty(prop);
  }
  for (csSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    csSGInput *input = node->GetInput(i);

    NodeGraphNodeValueProperty *prop = new NodeGraphNodeValueProperty(this);
    prop->SetShowValue(input->CanInputConst());
    prop->SetAnchorShow(input->CanInputNode());
    prop->SetName(QString(input->GetName().c_str()));
    prop->SetIdx(input->GetIdx());
    prop->Initialize();
    AddInputProperty(prop);
    m_valueProperties[i] = prop;
    m_inputAnchors[input->GetIdx()] = prop->GetAnchor();
  }

  for (csSize i = 0, in = node->GetNumberOfOutputs(); i < in; ++i)
  {
    csSGOutput *output = node->GetOutput(i);
    NodeGraphNodeValueProperty *prop = new NodeGraphNodeValueProperty(this);
    prop->SetShowValue(false);
    prop->SetAnchorShow(true);
    prop->SetName(QString(output->GetName().c_str()));
    prop->SetIdx(output->GetIdx());
    prop->Initialize();
    AddOutputProperty(prop);
    m_outputAnchors[output->GetIdx()] = prop->GetAnchor();
  }

  UpdateValues();

  Layout();

}

ShaderGraphEditorNode::~ShaderGraphEditorNode()
{
  CS_RELEASE(m_sgNode);
  CS_RELEASE(m_shaderGraph);
}

NodeGraphNodeAnchor* ShaderGraphEditorNode::GetInputAnchor(csUInt32 idx) const
{
  std::map<csUInt32, NodeGraphNodeAnchor*>::const_iterator it = m_inputAnchors.find(idx);
  if (it == m_inputAnchors.end())
  {
    return 0;
  }
  return it->second;
}


NodeGraphNodeAnchor* ShaderGraphEditorNode::GetOutputAnchor(csUInt32 idx) const
{
  std::map<csUInt32, NodeGraphNodeAnchor*>::const_iterator it = m_outputAnchors.find(idx);
  if (it == m_outputAnchors.end())
  {
    return 0;
  }
  return it->second;
}

csSGShaderGraph *ShaderGraphEditorNode::GetShaderGraph() const
{
  return m_shaderGraph;
}


csSGNode *ShaderGraphEditorNode::GetSGNode() const
{
  return m_sgNode;
}


void ShaderGraphEditorNode::UpdateValues()
{
  if (!m_sgNode)
  {
    return;
  }
  for (std::map<unsigned, NodeGraphNodeValueProperty*>::iterator it = m_valueProperties.begin();
    it != m_valueProperties.end();
    ++it) 
  {
    csSGInput *input = m_sgNode->GetInput(it->first);
    if (input && it->second)
    {
      it->second->SetValue(input->GetConst());
    }
  }

  csSGResourceNode *resourceNode = csQueryClass<csSGResourceNode>(m_sgNode);
  if (resourceNode)
  {
    NodeGraphNodeProperty *prop = GetInputProperty(IDX_TEXTURE_IMAGE);
    if (prop)
    {
      if (resourceNode->GetDefaultTextureResource() != m_texturePreviewResourceLocator)
      {
        NodeGraphNodeImageProperty *imgProp = static_cast<NodeGraphNodeImageProperty*>(prop);
        csResourceLocator locator(resourceNode->GetDefaultTextureResource(), "preview");
        EditorImage *editorImage = csResourceManager::Get()->Aquire<EditorImage>(locator);
        if (editorImage)
        {
          imgProp->SetImage(editorImage->GetImage());
        }
        m_texturePreviewResourceLocator = resourceNode->GetDefaultTextureResource();
      }
    }
    GetHeader()->SetSubName(QString(resourceNode->GetResourceName().c_str()));
  }
}

