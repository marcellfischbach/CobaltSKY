
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <AssetManager/EditorIcon.hh>
#include <Graph/Scene.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>


struct TypeMap
{
  QString name;
  QColor color;
  TypeMap(const QString &name, const QColor &color)
    : name(name)
    , color(color)
  {

  }
};


static TypeMap typeMap[] = {
  TypeMap("Math", QColor(128, 32, 32)),
  TypeMap("Var", QColor(32, 128, 32)),
  TypeMap("Const", QColor(32, 32, 128)),
  TypeMap("Assemble", QColor(32, 128, 128)),
  TypeMap("Texture", QColor(128, 128, 32)),
};

static QColor getTypeColor(const QString &type)
{
  size_t num = sizeof(typeMap) / sizeof(typeMap[0]);
  for (size_t i = 0; i < num; ++i)
  {
    if (typeMap[i].name == type)
    {
      return typeMap[i].color;
    }
  }
  return QColor(255, 255, 255);
}

static vkSGShaderGraph::InputType getSGInputType(const QString &key)
{
  if (key == "diffuse")
  {
    return vkSGShaderGraph::eIT_Diffuse;
  }
  else if (key == "roughness")
  {
    return vkSGShaderGraph::eIT_Roughness;
  }
  else if (key == "alpha")
  {
    return vkSGShaderGraph::eIT_Alpha;
  }
  else if (key == "normal")
  {
    return vkSGShaderGraph::eIT_Normal;
  }

  return vkSGShaderGraph::eIT_Undefined;
}



static void Split(const vkString &fullName, QString &category, QString &name)
{
  QStringList parts = QString(fullName.c_str()).split("/", QString::SkipEmptyParts);

  if (parts.size() == 0)
  {
    category = "Undefined";
    name = "NoName";
  }
  else if (parts.size() == 1)
  {
    category = "Undefined";
    name = parts[0];
  }
  else if (parts.size() >= 2)
  {
    category = parts[0];
    name = parts[1];
  }
}



shadergraph::SGNode::SGNode(vkSGNode *node)
  : shadergraph::Node(eT_Node)
  , m_node(0)
{
  VK_SET(m_node, node);
  if (!m_node)
  {
    return;
  }


  QString category, name;
  Split(m_node->GetName(), category, name);

  SetLabel(name);
  SetHeadlineColor(getTypeColor(category));

  m_res = vkQueryClass<vkSGResourceNode>(m_node);

  if (m_res && m_res->GetResourceType() == eSPT_Texture)
  {
    // TODO: Add image input element here
    /*
    SetShowImage(true);
    EditorIcon *icon = vkResourceManager::Get()->Load<EditorIcon>(vkResourceLocator(m_res->GetDefaultTextureResource().GetResourceFile(), "preview"));
    if (icon)
    {
      SetImage(icon->GetImage());
    }
    */
  }

  for (vkSize i = 0, in = m_node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    QString inputName(input->GetName().c_str());

    graph::AttribInputWidget *inputWidget  = new graph::AttribInputWidget();
    inputWidget->SetName(inputName);
    inputWidget->SetValue("");
    inputWidget->SetIndex(input->GetIdx());
    if (input->CanInputConst())
    {
      inputWidget->SetValue(QString::asprintf("%f", input->GetConst()));
    }

    // TODO: need to handle manual input later
    /*
    graph::Node::InputMode mode;
    if (input->CanInputConst())
    {
      mode = (graph::Node::InputMode)(mode | graph::Node::eIM_Const);
    }
    if (input->CanInputNode())
    {
      mode = (graph::Node::InputMode)(mode | graph::Node::eIM_Output);
    }
    AddInput(inputName, inputName, mode);
    */

    AddInput (inputWidget);
    m_inputs[inputName] = inputWidget;
  }




  for (vkSize i = 0, in = m_node->GetNumberOfOutputs(); i < in; ++i)
  {
    vkSGOutput *output = m_node->GetOutput(i);
    QString outputName(output->GetName().c_str());

    graph::AttribOutputWidget *outputWidget = new graph::AttribOutputWidget();
    outputWidget->SetName(outputName);
    outputWidget->SetIndex(output->GetIdx());
    if (outputName == "r")
    {
      outputWidget->SetColor(QColor(128, 0, 0));
    }
    else if (outputName == "g")
    {
      outputWidget->SetColor(QColor(0, 128, 0));
    }
    else if (outputName == "b")
    {
      outputWidget->SetColor(QColor(0, 0, 128));
    }

    AddOutput(outputWidget);
    m_outputs[outputName] = outputWidget;
  }

}


void shadergraph::SGNode::UpdateResource()
{
  if (m_res && m_res->GetResourceType() == eSPT_Texture)
  {
    /*
    EditorIcon *icon = vkResourceManager::Get()->Load<EditorIcon>(vkResourceLocator(m_res->GetDefaultTextureResource().GetResourceFile(), "preview"));
    if (icon)
    {
      SetImage(icon->GetImage());
    }
    else
    {
      SetImage(QImage(":/icons/Resources/NoIcon64.png"));
    }
    */
  }

}
