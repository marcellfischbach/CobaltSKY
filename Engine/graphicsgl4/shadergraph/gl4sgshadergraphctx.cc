
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>


cs::ShaderGraphCtx::ShaderGraphCtx(cs::ShaderGraphGL4 *graph)
  : m_graph(graph)
  , m_code("")
  , m_variableCounter(0)
{

}

cs::SGNodeGL4 *cs::ShaderGraphCtx::GetNode(cs::SGNode *node)
{
  if (!node)
  {
    return 0;
  }

  cs::SGNodeGL4 *glNode = m_nodes[node];
  if (!glNode)
  {
    const cs::Class *cls = node->GetClass();
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

cs::SGNodeGL4 *cs::ShaderGraphCtx::GetNode(cs::SGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return GetNode(output->GetNode());
}

cs::SGNodeGL4 *cs::ShaderGraphCtx::GetNode(cs::SGInput *input)
{
  if (!input)
  {
    return 0;
  }

  return GetNode(input->GetInput());
}

void cs::ShaderGraphCtx::Evaluate(cs::SGNodeGL4 *node)
{
  if (node)
  {
    node->Evaluate(*this);
  }
}

void cs::ShaderGraphCtx::Evaluate(cs::SGNode *node)
{
  Evaluate(GetNode(node));
}


void cs::ShaderGraphCtx::Evaluate(cs::SGOutput *output)
{
  Evaluate(GetNode(output));
}

void cs::ShaderGraphCtx::Evaluate(cs::SGInput *input)
{
  Evaluate(GetNode(input));
}


void cs::ShaderGraphCtx::EvaluateInline(cs::SGNodeGL4 *node)
{
  if (node)
  {
    node->EvaluateInline(*this);
  }
}

void cs::ShaderGraphCtx::EvaluateInline(cs::SGNode *node)
{
  EvaluateInline(GetNode(node));
}


void cs::ShaderGraphCtx::EvaluateInline(cs::SGOutput *output)
{
  EvaluateInline(GetNode(output));
}

void cs::ShaderGraphCtx::EvaluateInline(cs::SGInput *input)
{
  EvaluateInline(GetNode(input));
}



std::string cs::ShaderGraphCtx::CreateCode(cs::SGOutput *output)
{
  cs::SGNodeGL4 *node = GetNode(output->GetNode());
  EvaluateInline(node);
  Evaluate(node);

  std::string code = m_code;
  code += std::string("\n") + GetFullOutputValue(output).c_str() + std::string("\n");

  return code;
}

void cs::ShaderGraphCtx::EvaluateInlines(std::set<cs::SGOutput*> outputs)
{
  for (cs::SGOutput *output : outputs)
  {
    EvaluateInline(output);
  }
}

void cs::ShaderGraphCtx::GenerateCode(std::set<cs::SGOutput*> outputs)
{
  m_code = "";
  for (cs::SGOutput *output : outputs)
  {
    cs::SGNode *node = output->GetNode();
    Evaluate(output);
  }
}

void cs::ShaderGraphCtx::AddExpression(const std::string &expression)
{
  m_code += expression + std::string(";\n");
}

std::string cs::ShaderGraphCtx::AddAssignment(const std::string &type, const std::string &statement)
{
  std::string var = GetNextVariable();
  std::string expression = type + std::string(" ") + var + std::string(" = ") + statement;
  AddExpression(expression);
  return var;
}


std::string cs::ShaderGraphCtx::GetNextVariable()
{
  static char buffer[64];
  m_variableCounter++;
  snprintf(buffer, 64, "v%03u", m_variableCounter);
  return std::string(buffer);
}


void cs::ShaderGraphCtx::AddBinding(cs::SGNode *node, const std::string &variableType, const std::string &variableName)
{
  ExternalBinding binding;
  binding.variableName = variableName;
  binding.node = node;
  binding.variableType = variableType;
  m_unisformBindingNames[variableName] = binding;;
}

bool cs::ShaderGraphCtx::IsBindingApplyingFor(const std::string &bindingName, cs::SGNode *node) const
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
    cs::SGInput *input = node->GetInput(i);
    if (IsBindingApplyingFor(bindingName, input))
    {
      return true;
    }
  }
  return false;
}

bool cs::ShaderGraphCtx::IsBindingApplyingFor(const std::string &bindingName, cs::SGOutput *output) const
{
  if (!output)
  {
    return false;
  }
  return IsBindingApplyingFor(bindingName, output->GetNode());
}

bool cs::ShaderGraphCtx::IsBindingApplyingFor(const std::string &bindingName, cs::SGInput *input) const
{
  if (!input)
  {
    return false;
  }

  return IsBindingApplyingFor(bindingName, input->GetInput());
}

std::set<cs::ShaderGraphCtx::ExternalBinding> cs::ShaderGraphCtx::GetBindingsFor(std::set<cs::SGOutput*> outputs) const
{
  std::set<ExternalBinding> result;
  std::map<std::string, ExternalBinding>::const_iterator it;
  for (it = m_unisformBindingNames.begin(); it != m_unisformBindingNames.end(); ++it)
  {
    for (cs::SGOutput* output : outputs)
    {
      if (IsBindingApplyingFor(it->second.variableName, output))
      {
        result.insert(it->second);
      }
    }
  }

  return result;
}

void cs::ShaderGraphCtx::SetDefaultTextureCoordinate(const std::string &defaultTextureCoordinate)
{
  m_defaultTextureCoordinate = defaultTextureCoordinate;
}

const std::string &cs::ShaderGraphCtx::GetDefaultTextureCoordinate() const
{
  return m_defaultTextureCoordinate;
}

void cs::ShaderGraphCtx::SetOutputValue(cs::SGOutput *output, const std::string &value)
{
  if (output)
  {
    m_outputValue[output] = value;
  }
}

bool cs::ShaderGraphCtx::HasInputValue(cs::SGInput *input) const
{
  return HasOutputValue(input ? input->GetInput() : 0);
}

bool cs::ShaderGraphCtx::HasOutputValue(cs::SGOutput *output) const
{
  std::map<cs::SGOutput *, std::string>::const_iterator it = m_outputValue.find(output);
  return it != m_outputValue.end();
}

std::string cs::ShaderGraphCtx::GetOutputValue(cs::SGOutput *output)
{
  std::map<cs::SGOutput *, std::string>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return std::string("<inval>");
  }
  return it->second;
}

std::string cs::ShaderGraphCtx::GetInputValue(cs::SGInput *input)
{
  return GetOutputValue(input ? input->GetInput() : 0);
}

std::string cs::ShaderGraphCtx::GetFullOutputValue(cs::SGOutput *output)
{
  std::map<cs::SGOutput *, std::string>::iterator it = m_outputValue.find(output);
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

std::string cs::ShaderGraphCtx::GetFullInputValue(cs::SGInput *input)
{
  if (input->GetInput() == 0 && input->CanInputConst())
  {
    return std::to_string(input->GetConst());
  }
  return GetFullOutputValue(input ? input->GetInput() : 0);
}
