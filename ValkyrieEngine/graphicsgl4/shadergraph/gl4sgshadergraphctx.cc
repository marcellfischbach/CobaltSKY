
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksginput.hh>
#include <valkyrie/graphics/shadergraph/vksgoutput.hh>


vkShaderGraphCtx::vkShaderGraphCtx(vkShaderGraphGL4 *graph)
  : m_graph(graph)
  , m_code("")
  , m_variableCounter(0)
{

}

vkSGNodeGL4 *vkShaderGraphCtx::GetNode(vkSGNode *node)
{
  if (!node)
  {
    return 0;
  }

  vkSGNodeGL4 *glNode = m_nodes[node];
  if (!glNode)
  {
    const vkClass *cls = node->GetClass();
    glNode = m_graph->CreateNode(cls);
    if (glNode == 0)
    {
      return 0;
    }

    glNode->SetNode(node);
    m_nodes[node] = glNode;
  }
  return glNode;
}

vkSGNodeGL4 *vkShaderGraphCtx::GetNode(vkSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return GetNode(output->GetNode());
}

vkSGNodeGL4 *vkShaderGraphCtx::GetNode(vkSGInput *input)
{
  if (!input)
  {
    return 0;
  }

  return GetNode(input->GetInput());
}

void vkShaderGraphCtx::Evaluate(vkSGNodeGL4 *node)
{
  if (node)
  {
    node->Evaluate(*this);
  }
}

void vkShaderGraphCtx::Evaluate(vkSGNode *node)
{
  Evaluate(GetNode(node));
}


void vkShaderGraphCtx::Evaluate(vkSGOutput *output)
{
  Evaluate(GetNode(output));
}

void vkShaderGraphCtx::Evaluate(vkSGInput *input)
{
  Evaluate(GetNode(input));
}


void vkShaderGraphCtx::EvaluateInline(vkSGNodeGL4 *node)
{
  if (node)
  {
    node->EvaluateInline(*this);
  }
}

void vkShaderGraphCtx::EvaluateInline(vkSGNode *node)
{
  EvaluateInline(GetNode(node));
}


void vkShaderGraphCtx::EvaluateInline(vkSGOutput *output)
{
  EvaluateInline(GetNode(output));
}

void vkShaderGraphCtx::EvaluateInline(vkSGInput *input)
{
  EvaluateInline(GetNode(input));
}



vkString vkShaderGraphCtx::CreateCode(vkSGOutput *output)
{
  vkSGNodeGL4 *node = GetNode(output->GetNode());
  EvaluateInline(node);
  Evaluate(node);

  vkString code = m_code;
  code += vkString("\n") + GetFullOutputValue(output).c_str() + vkString("\n");

  return code;
}

void vkShaderGraphCtx::EvaluateInlines(std::set<vkSGOutput*> outputs)
{
  for (vkSGOutput *output : outputs)
  {
    EvaluateInline(output);
  }
}

void vkShaderGraphCtx::GenerateCode(std::set<vkSGOutput*> outputs)
{
  m_code = "";
  for (vkSGOutput *output : outputs)
  {
    vkSGNode *node = output->GetNode();
    Evaluate(output);
  }
}

void vkShaderGraphCtx::AddExpression(const vkString &expression)
{
  m_code += expression + vkString(";\n");
}

vkString vkShaderGraphCtx::AddAssignment(const vkString &type, const vkString &statement)
{
  vkString var = GetNextVariable();
  vkString expression = type + vkString(" ") + var + vkString(" = ") + statement;
  AddExpression(expression);
  return var;
}


vkString vkShaderGraphCtx::GetNextVariable()
{
  static char buffer[64];
  m_variableCounter++;
  snprintf(buffer, 64, "v%03u", m_variableCounter);
  return vkString(buffer);
}


void vkShaderGraphCtx::AddBinding(vkSGNode *node, const vkString &variableType, const vkString &variableName)
{
  ExternalBinding binding;
  binding.variableName = variableName;
  binding.node = node;
  binding.variableType = variableType;
  m_unisformBindingNames[variableName] = binding;;
}

bool vkShaderGraphCtx::IsBindingApplyingFor(const vkString &bindingName, vkSGNode *node) const
{
  if (!node)
  {
    return false;
  }

  std::map<vkString, ExternalBinding>::const_iterator it = m_unisformBindingNames.find(bindingName);
  if (it != m_unisformBindingNames.end() && it->second.node == node)
  {
    return true;
  }

  for (vkSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = node->GetInput(i);
    if (IsBindingApplyingFor(bindingName, input))
    {
      return true;
    }
  }
  return false;
}

bool vkShaderGraphCtx::IsBindingApplyingFor(const vkString &bindingName, vkSGOutput *output) const
{
  if (!output)
  {
    return false;
  }
  return IsBindingApplyingFor(bindingName, output->GetNode());
}

bool vkShaderGraphCtx::IsBindingApplyingFor(const vkString &bindingName, vkSGInput *input) const
{
  if (!input)
  {
    return false;
  }

  return IsBindingApplyingFor(bindingName, input->GetInput());
}

std::set<vkShaderGraphCtx::ExternalBinding> vkShaderGraphCtx::GetBindingsFor(std::set<vkSGOutput*> outputs) const
{
  std::set<ExternalBinding> result;
  std::map<vkString, ExternalBinding>::const_iterator it;
  for (it = m_unisformBindingNames.begin(); it != m_unisformBindingNames.end(); ++it)
  {
    for (vkSGOutput* output : outputs)
    {
      if (IsBindingApplyingFor(it->second.variableName, output))
      {
        result.insert(it->second);
      }
    }
  }

  return result;
}

void vkShaderGraphCtx::SetDefaultTextureCoordinate(const vkString &defaultTextureCoordinate)
{
  m_defaultTextureCoordinate = defaultTextureCoordinate;
}

const vkString &vkShaderGraphCtx::GetDefaultTextureCoordinate() const
{
  return m_defaultTextureCoordinate;
}

void vkShaderGraphCtx::SetOutputValue(vkSGOutput *output, const vkString &value)
{
  if (output)
  {
    m_outputValue[output] = value;
  }
}

bool vkShaderGraphCtx::HasInputValue(vkSGInput *input) const
{
  return HasOutputValue(input ? input->GetInput() : 0);
}

bool vkShaderGraphCtx::HasOutputValue(vkSGOutput *output) const
{
  std::map<vkSGOutput *, vkString>::const_iterator it = m_outputValue.find(output);
  return it != m_outputValue.end();
}

vkString vkShaderGraphCtx::GetOutputValue(vkSGOutput *output)
{
  std::map<vkSGOutput *, vkString>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return vkString("<inval>");
  }
  return it->second;
}

vkString vkShaderGraphCtx::GetInputValue(vkSGInput *input)
{
  return GetOutputValue(input ? input->GetInput() : 0);
}

vkString vkShaderGraphCtx::GetFullOutputValue(vkSGOutput *output)
{
  std::map<vkSGOutput *, vkString>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return vkString("<inval>");
  }

  vkString res = it->second;

  if (output->GetAttr().length() != 0)
  {
    res = res += vkString(".") + output->GetAttr();
  }
  return res;
}

vkString vkShaderGraphCtx::GetFullInputValue(vkSGInput *input)
{
  if (input->GetInput() == 0 && input->CanInputConst())
  {
    return std::to_string(input->GetConst());
  }
  return GetFullOutputValue(input ? input->GetInput() : 0);
}
