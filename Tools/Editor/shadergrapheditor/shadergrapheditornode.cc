

#include <shadergrapheditor/shadergrapheditornode.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <nodegraph/nodegraphnodeheader.hh>
#include <nodegraph/nodegraphnodeimageproperty.hh>
#include <nodegraph/nodegraphnodevalueproperty.hh>
#include <QMap>
#include <QColor>



ShaderGraphEditorNode::ShaderGraphEditorNode(vkSGShaderGraph *shaderGraph)
  : NodeGraphNode()
  , m_shaderGraph(0)
  , m_sgNode(0)
{
  VK_SET(m_shaderGraph, shaderGraph);
  GetHeader()->SetName("Shader");

  NodeGraphNodeValueProperty *propDiffuse = new NodeGraphNodeValueProperty(this);
  propDiffuse->SetAnchorShow(true);
  propDiffuse->SetShowValue(false);
  propDiffuse->SetName("Diffuse");
  propDiffuse->SetIdx(vkSGShaderGraph::eIT_Diffuse);
  propDiffuse->Initialize();
  m_inputAnchors[vkSGShaderGraph::eIT_Diffuse] = propDiffuse->GetAnchor();

  NodeGraphNodeValueProperty *propAlpha = new NodeGraphNodeValueProperty(this);
  propAlpha->SetAnchorShow(true);
  propAlpha->SetShowValue(false);
  propAlpha->SetName("Alpha");
  propAlpha->SetIdx(vkSGShaderGraph::eIT_Alpha);
  propAlpha->Initialize();
  m_inputAnchors[vkSGShaderGraph::eIT_Alpha] = propAlpha->GetAnchor();

  NodeGraphNodeValueProperty *propRoughness = new NodeGraphNodeValueProperty(this);
  propRoughness->SetAnchorShow(true);
  propRoughness->SetShowValue(false);
  propRoughness->SetName("Roughness");
  propRoughness->SetIdx(vkSGShaderGraph::eIT_Roughness);
  propRoughness->Initialize();
  m_inputAnchors[vkSGShaderGraph::eIT_Roughness] = propRoughness->GetAnchor();

  NodeGraphNodeValueProperty *propNormal = new NodeGraphNodeValueProperty(this);
  propNormal->SetAnchorShow(true);
  propNormal->SetShowValue(false);
  propNormal->SetName("Normal");
  propNormal->SetIdx(vkSGShaderGraph::eIT_Normal);
  propNormal->Initialize();
  m_inputAnchors[vkSGShaderGraph::eIT_Normal] = propNormal->GetAnchor();


  AddInputProperty(propDiffuse);
  AddInputProperty(propAlpha);
  AddInputProperty(propRoughness);
  AddInputProperty(propNormal);
  Layout();
}

ShaderGraphEditorNode::ShaderGraphEditorNode(vkSGNode *node)
  : NodeGraphNode()
  , m_shaderGraph(0)
  , m_sgNode(0)
{
  VK_SET(m_sgNode, node);
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
  vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(node);
  if (resource)
  {
    GetHeader()->SetSubName(QString(resource->GetResourceName().c_str()));

    NodeGraphNodeImageProperty *prop = new NodeGraphNodeImageProperty(this);

    prop->SetIdx(-1);
    AddInputProperty(prop);
  }
  for (vkSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = node->GetInput(i);

    NodeGraphNodeValueProperty *prop = new NodeGraphNodeValueProperty(this);
    prop->SetShowValue(input->CanInputConst());
    prop->SetAnchorShow(input->CanInputNode());
    prop->SetValue(input->GetConst());
    prop->SetName(QString(input->GetName().c_str()));
    prop->SetIdx(input->GetIdx());
    prop->Initialize();
    AddInputProperty(prop);
    m_valueProperties[i] = prop;
    m_inputAnchors[input->GetIdx()] = prop->GetAnchor();
  }

  for (vkSize i = 0, in = node->GetNumberOfOutputs(); i < in; ++i)
  {
    vkSGOutput *output = node->GetOutput(i);
    NodeGraphNodeValueProperty *prop = new NodeGraphNodeValueProperty(this);
    prop->SetShowValue(false);
    prop->SetAnchorShow(true);
    prop->SetName(QString(output->GetName().c_str()));
    prop->SetIdx(output->GetIdx());
    prop->Initialize();
    AddOutputProperty(prop);
    m_outputAnchors[output->GetIdx()] = prop->GetAnchor();
  }

  Layout();

}

ShaderGraphEditorNode::~ShaderGraphEditorNode()
{
  VK_RELEASE(m_sgNode);
  VK_RELEASE(m_shaderGraph);
}

NodeGraphNodeAnchor* ShaderGraphEditorNode::GetInputAnchor(vkUInt32 idx) const
{
  std::map<vkUInt32, NodeGraphNodeAnchor*>::const_iterator it = m_inputAnchors.find(idx);
  if (it == m_inputAnchors.end())
  {
    return 0;
  }
  return it->second;
}


NodeGraphNodeAnchor* ShaderGraphEditorNode::GetOutputAnchor(vkUInt32 idx) const
{
  std::map<vkUInt32, NodeGraphNodeAnchor*>::const_iterator it = m_outputAnchors.find(idx);
  if (it == m_outputAnchors.end())
  {
    return 0;
  }
  return it->second;
}

vkSGShaderGraph *ShaderGraphEditorNode::GetShaderGraph() const
{
  return m_shaderGraph;
}


vkSGNode *ShaderGraphEditorNode::GetSGNode() const
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
    vkSGInput *input = m_sgNode->GetInput(it->first);
    if (input && it->second)
    {
      it->second->SetValue(input->GetConst());
    }
  }

  vkSGResourceNode *resourceNode = vkQueryClass<vkSGResourceNode>(m_sgNode);
  if (resourceNode)
  {
    GetHeader()->SetSubName(QString(resourceNode->GetResourceName().c_str()));
  }
}

