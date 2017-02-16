

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

  NodeGraphNodeValueProperty *propAlpha = new NodeGraphNodeValueProperty(this);
  propAlpha->SetAnchorShow(true);
  propAlpha->SetShowValue(false);
  propAlpha->SetName("Alpha");

  NodeGraphNodeValueProperty *propRoughness = new NodeGraphNodeValueProperty(this);
  propRoughness->SetAnchorShow(true);
  propRoughness->SetShowValue(false);
  propRoughness->SetName("Roughness");

  NodeGraphNodeValueProperty *propNormal = new NodeGraphNodeValueProperty(this);
  propNormal->SetAnchorShow(true);
  propNormal->SetShowValue(false);
  propNormal->SetName("Normal");


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
    NodeGraphNodeImageProperty *prop = new NodeGraphNodeImageProperty(this);
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
    AddInputProperty(prop);
  }

  for (vkSize i = 0, in = node->GetNumberOfOutputs(); i < in; ++i)
  {
    vkSGOutput *output = node->GetOutput(i);
    NodeGraphNodeValueProperty *prop = new NodeGraphNodeValueProperty(this);
    prop->SetShowValue(false);
    prop->SetAnchorShow(true);
    prop->SetName(QString(output->GetName().c_str()));
    AddOutputProperty(prop);
  }

  Layout();

}

ShaderGraphEditorNode::~ShaderGraphEditorNode()
{
  VK_RELEASE(m_sgNode);
  VK_RELEASE(m_shaderGraph);
}


vkSGShaderGraph *ShaderGraphEditorNode::GetShaderGraph() const
{
  return m_shaderGraph;
}


vkSGNode *ShaderGraphEditorNode::GetSGNode() const
{
  return m_sgNode;
}

