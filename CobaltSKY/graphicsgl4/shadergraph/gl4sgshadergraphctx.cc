
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>


csShaderGraphCtx::csShaderGraphCtx(csShaderGraphGL4 *graph)
  : m_graph(graph)
  , m_code("")
  , m_variableCounter(0)
{

}

csSGNodeGL4 *csShaderGraphCtx::GetNode(csSGNode *node)
{
  if (!node)
  {
    return 0;
  }

  csSGNodeGL4 *glNode = m_nodes[node];
  if (!glNode)
  {
    const csClass *cls = node->GetClass();
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

csSGNodeGL4 *csShaderGraphCtx::GetNode(csSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return GetNode(output->GetNode());
}

csSGNodeGL4 *csShaderGraphCtx::GetNode(csSGInput *input)
{
  if (!input)
  {
    return 0;
  }

  return GetNode(input->GetInput());
}

void csShaderGraphCtx::Evaluate(csSGNodeGL4 *node)
{
  if (node)
  {
    node->Evaluate(*this);
  }
}

void csShaderGraphCtx::Evaluate(csSGNode *node)
{
  Evaluate(GetNode(node));
}


void csShaderGraphCtx::Evaluate(csSGOutput *output)
{
  Evaluate(GetNode(output));
}

void csShaderGraphCtx::Evaluate(csSGInput *input)
{
  Evaluate(GetNode(input));
}


void csShaderGraphCtx::EvaluateInline(csSGNodeGL4 *node)
{
  if (node)
  {
    node->EvaluateInline(*this);
  }
}

void csShaderGraphCtx::EvaluateInline(csSGNode *node)
{
  EvaluateInline(GetNode(node));
}


void csShaderGraphCtx::EvaluateInline(csSGOutput *output)
{
  EvaluateInline(GetNode(output));
}

void csShaderGraphCtx::EvaluateInline(csSGInput *input)
{
  EvaluateInline(GetNode(input));
}



std::string csShaderGraphCtx::CreateCode(csSGOutput *output)
{
  csSGNodeGL4 *node = GetNode(output->GetNode());
  EvaluateInline(node);
  Evaluate(node);

  std::string code = m_code;
  code += std::string("\n") + GetFullOutputValue(output).c_str() + std::string("\n");

  return code;
}

void csShaderGraphCtx::EvaluateInlines(std::set<csSGOutput*> outputs)
{
  for (csSGOutput *output : outputs)
  {
    EvaluateInline(output);
  }
}

void csShaderGraphCtx::GenerateCode(std::set<csSGOutput*> outputs)
{
  m_code = "";
  for (csSGOutput *output : outputs)
  {
    csSGNode *node = output->GetNode();
    Evaluate(output);
  }
}

void csShaderGraphCtx::AddExpression(const std::string &expression)
{
  m_code += expression + std::string(";\n");
}

std::string csShaderGraphCtx::AddAssignment(const std::string &type, const std::string &statement)
{
  std::string var = GetNextVariable();
  std::string expression = type + std::string(" ") + var + std::string(" = ") + statement;
  AddExpression(expression);
  return var;
}


std::string csShaderGraphCtx::GetNextVariable()
{
  static char buffer[64];
  m_variableCounter++;
  snprintf(buffer, 64, "v%03u", m_variableCounter);
  return std::string(buffer);
}


void csShaderGraphCtx::AddBinding(csSGNode *node, const std::string &variableType, const std::string &variableName)
{
  ExternalBinding binding;
  binding.variableName = variableName;
  binding.node = node;
  binding.variableType = variableType;
  m_unisformBindingNames[variableName] = binding;;
}

bool csShaderGraphCtx::IsBindingApplyingFor(const std::string &bindingName, csSGNode *node) const
{
  if (!node)
  {
    return false;
  }

  std::map<std::string, ExternalBinding>::const_iterator it = m_unisformBindingNames.find(bindingName);
  if (it != m_unisformBindingNames.end() && it->second.node == node)
  {
    return true;
  }

  for (csSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    csSGInput *input = node->GetInput(i);
    if (IsBindingApplyingFor(bindingName, input))
    {
      return true;
    }
  }
  return false;
}

bool csShaderGraphCtx::IsBindingApplyingFor(const std::string &bindingName, csSGOutput *output) const
{
  if (!output)
  {
    return false;
  }
  return IsBindingApplyingFor(bindingName, output->GetNode());
}

bool csShaderGraphCtx::IsBindingApplyingFor(const std::string &bindingName, csSGInput *input) const
{
  if (!input)
  {
    return false;
  }

  return IsBindingApplyingFor(bindingName, input->GetInput());
}

std::set<csShaderGraphCtx::ExternalBinding> csShaderGraphCtx::GetBindingsFor(std::set<csSGOutput*> outputs) const
{
  std::set<ExternalBinding> result;
  std::map<std::string, ExternalBinding>::const_iterator it;
  for (it = m_unisformBindingNames.begin(); it != m_unisformBindingNames.end(); ++it)
  {
    for (csSGOutput* output : outputs)
    {
      if (IsBindingApplyingFor(it->second.variableName, output))
      {
        result.insert(it->second);
      }
    }
  }

  return result;
}

void csShaderGraphCtx::SetDefaultTextureCoordinate(const std::string &defaultTextureCoordinate)
{
  m_defaultTextureCoordinate = defaultTextureCoordinate;
}

const std::string &csShaderGraphCtx::GetDefaultTextureCoordinate() const
{
  return m_defaultTextureCoordinate;
}

void csShaderGraphCtx::SetOutputValue(csSGOutput *output, const std::string &value)
{
  if (output)
  {
    m_outputValue[output] = value;
  }
}

bool csShaderGraphCtx::HasInputValue(csSGInput *input) const
{
  return HasOutputValue(input ? input->GetInput() : 0);
}

bool csShaderGraphCtx::HasOutputValue(csSGOutput *output) const
{
  std::map<csSGOutput *, std::string>::const_iterator it = m_outputValue.find(output);
  return it != m_outputValue.end();
}

std::string csShaderGraphCtx::GetOutputValue(csSGOutput *output)
{
  std::map<csSGOutput *, std::string>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return std::string("<inval>");
  }
  return it->second;
}

std::string csShaderGraphCtx::GetInputValue(csSGInput *input)
{
  return GetOutputValue(input ? input->GetInput() : 0);
}

std::string csShaderGraphCtx::GetFullOutputValue(csSGOutput *output)
{
  std::map<csSGOutput *, std::string>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return std::string("<inval>");
  }

  std::string res = it->second;

  if (output->GetAttr().length() != 0)
  {
    res = res += std::string(".") + output->GetAttr();
  }
  return res;
}

std::string csShaderGraphCtx::GetFullInputValue(csSGInput *input)
{
  if (input->GetInput() == 0 && input->CanInputConst())
  {
    return std::to_string(input->GetConst());
  }
  return GetFullOutputValue(input ? input->GetInput() : 0);
}
