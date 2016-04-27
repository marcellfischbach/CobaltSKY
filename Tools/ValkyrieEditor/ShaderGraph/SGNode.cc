
#include <ShaderGraph/SGNode.hh>
#include <ShaderGraph/SGShaderGraphNode.hh>
#include <Graph/Connection.hh>
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



shadergraph::SGNode::SGNode(const vkClass *nodeClass)
  : shadergraph::Node(eT_Node)
  , m_nodeClass(nodeClass)
{
  vkSGNode *node = nodeClass->CreateInstance<vkSGNode>();
  if (!node)
  {
    return;
  }

  SetMinWidth(100.0f);

  QString category, name;
  Split(node->GetName(), category, name);

  SetLabel(name);
  SetBackgroundColor(getTypeColor(category));

  vkSGResourceNode *res = vkQueryClass<vkSGResourceNode>(node);

  SetHasName(res != 0);
  if (HasName())
  {
    QString name = "";
    if (res->GetResourceName().length() == 0)
    {
      static unsigned nameCounter = 1;
      name = QString("Var%1").arg(nameCounter++);
    }
    else
    {
      name = QString(res->GetResourceName().c_str());
    }
    SetName(name);
  }

  for (vkSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = node->GetInput(i);
    QString inputName(input->GetName().c_str());

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
  }
  for (vkSize i = 0, in = node->GetNumberOfOutputs(); i < in; ++i)
  {
    vkSGOutput *output = node->GetOutput(i);
    QString outputName(output->GetName().c_str());

    AddOutput(outputName, outputName);
  }

  node->Release();
}

void shadergraph::SGNode::AddConnection(graph::NodeConnection* connection)
{
  if (connection->GetInputNode() == this)
  {
    if (GetScene())
    {
      GetScene()->DisconnectInput(this, connection->GetInputIdx());
    }
  }
  shadergraph::Node::AddConnection(connection);
}

void shadergraph::SGNode::RemoveConnection(graph::NodeConnection* connection)
{
  shadergraph::Node::RemoveConnection(connection);
}

void shadergraph::SGNode::RemoveAllConnections()
{
  /*
  for (auto it : m_connections)
  {
    graph::NodeConnection* connection = it;
    if (connection->GetOutputNode() == this)
    {
      graph::Node *grNode = connection->GetInputNode();
      if (!grNode)
      {
        continue;
      }
      Node *sgNode = static_cast<Node*>(grNode);
      if (sgNode->GetType() == SGNode::eT_Node)
      {
        vkSGNode *inputNode = static_cast<SGNode*>(sgNode)->GetNode();
        if (inputNode)
        {
          inputNode->SetInput(connection->GetInputIdx(), 0, 0);
        }
      }
      else if (sgNode->GetType() == SGNode::eT_ShaderGraph)
      {
        vkSGShaderGraph *shaderGraph = static_cast<SGShaderGraphNode*>(sgNode)->GetShaderGraph();
        shaderGraph->SetInput((vkSGShaderGraph::InputType)connection->GetInputIdx(), 0);
      }
    }
  }

  for (vkSize i = 0, in = m_node->GetNumberOfInputs(); i < in; ++i)
  {
    // remove the input connection
    m_node->SetInput(i, 0, 0);
  }
  */
  shadergraph::Node::RemoveAllConnections();
}
