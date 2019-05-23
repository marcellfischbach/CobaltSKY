
#include <cobalt/graphics/shadergraph/cssgnode.hh>




cs::SGNode::SGNode()
  : cs::Object()
{
}

cs::SGNode::~SGNode()
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    delete m_inputs[i];
  }
  m_inputs.clear();

  for (size_t i = 0, in = m_outputs.size(); i < in; ++i)
  {
    delete m_outputs[i];
  }
  m_outputs.clear();

}

cs::SGNode *cs::SGNode::Copy(cs::SGNode *node) const
{
  node->m_name = m_name;
  return node;
}

void cs::SGNode::AddInput(cs::SGInput *input)
{
  if (input)
  {
    input->m_node = this;
    input->SetIdx((csUInt32)m_inputs.size());
    m_inputs.push_back(input);
  }
}

csSize cs::SGNode::GetNumberOfInputs() const
{
  return m_inputs.size();
}

cs::SGInput *cs::SGNode::GetInput(csSize idx) const
{
  if (idx >= m_inputs.size())
  {
    return 0;
  }
  return m_inputs[idx];
}

cs::SGInput *cs::SGNode::GetInput(const std::string &name) const
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    cs::SGInput *input = m_inputs[i];
    if (name == input->GetName())
    {
      return input;
    }
  }
  return 0;
}

cs::eSGDataType cs::SGNode::GetInputDataType(csSize idx) const
{
  cs::SGInput *input = GetInput(idx);
  if (!input)
  {
    return cs::eSGDT_Inval;
  }
  return input->GetDataType();
}

cs::eSGDataType cs::SGNode::GetInputDataType(const std::string &name) const
{
  cs::SGInput *input = GetInput(name);
  if (!input)
  {
    return cs::eSGDT_Inval;
  }
  return input->GetDataType();
}

void cs::SGNode::SetOutputDataType(csSize idx, cs::eSGDataType dataType)
{
  cs::SGOutput *output = GetOutput(idx);
  if (output)
  {
    output->SetDataType(dataType);
  }
}


void cs::SGNode::SetOutputDataType(const std::string &name, cs::eSGDataType dataType)
{
  cs::SGOutput *output = GetOutput(name);
  if (output)
  {
    output->SetDataType(dataType);
  }
}


void cs::SGNode::SetInput(int inputIdx, cs::SGNode *node, int outputIdx)
{
  cs::SGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  cs::SGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputIdx);
  }

  input->SetInput(output);
}

void cs::SGNode::SetInput(const std::string &inputName, cs::SGNode *node, int outputIdx)
{
  cs::SGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  cs::SGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputIdx);
  }

  input->SetInput(output);
}


void cs::SGNode::SetInput(int inputIdx, cs::SGNode *node, const std::string &outputName)
{
  cs::SGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  cs::SGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputName);
  }

  input->SetInput(output);
}

void cs::SGNode::SetInput(const std::string &inputName, cs::SGNode *node, const std::string &outputName)
{
  cs::SGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  cs::SGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputName);
  }

  input->SetInput(output);
}

void cs::SGNode::SetInput(int idx, float constFloat)
{
  cs::SGInput *input = GetInput(idx);
  if (!input)
  {
    return;
  }

  input->SetConst(constFloat);
}

void cs::SGNode::SetInput(const std::string &inputName, float constFloat)
{
  cs::SGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }

  input->SetConst(constFloat);
}

cs::SGNode *cs::SGNode::GetInputNode(csSize idx)
{
  cs::SGInput *input = GetInput(idx);
  return (input ? input->GetInputNode() : 0);
}

cs::SGNode *cs::SGNode::GetInputNode(const std::string &name)
{
  cs::SGInput *input = GetInput(name);
  return input ? input->GetInputNode() : 0;
}

void cs::SGNode::AddOutput(cs::SGOutput *output)
{
  if (output)
  {
    output->m_node = this;
    output->SetIdx((csUInt32)m_outputs.size());
    m_outputs.push_back(output);
  }
}

csSize cs::SGNode::GetNumberOfOutputs() const
{
  return m_outputs.size();
}

cs::SGOutput *cs::SGNode::GetOutput(csSize idx)
{
  if (idx >= m_outputs.size())
  {
    return 0;
  }
  return m_outputs[idx];
}

cs::SGOutput *cs::SGNode::GetOutput(const std::string &name)
{
  for (size_t i = 0, in = m_outputs.size(); i < in; ++i)
  {
    cs::SGOutput *output = m_outputs[i];
    if (name == output->GetName())
    {
      return output;
    }
  }
  return 0;
}

bool cs::SGNode::NotInvalid(cs::eSGDataType dt) const
{
  return dt != cs::eSGDT_Inval;
}

bool cs::SGNode::ScalarType(cs::eSGDataType dt) const
{
  return (dt & cs::eSGDT_ScalarTypes) == dt;
}

bool cs::SGNode::FloatType(cs::eSGDataType dt) const
{
  return (dt & cs::eSGDT_FloatTypes) == dt;
}

bool cs::SGNode::IntType(cs::eSGDataType dt) const
{
  return (dt & cs::eSGDT_IntTypes) == dt;
}

bool cs::SGNode::NotInvalid(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return NotInvalid(dtA) && NotInvalid(dtB);
}

bool cs::SGNode::ScalarType(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return ScalarType(dtA) && ScalarType(dtB);
}


bool cs::SGNode::FloatType(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return FloatType(dtA) && FloatType(dtB);
}


bool cs::SGNode::IntType(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return IntType(dtA) && IntType(dtB);
}

bool cs::SGNode::SameType(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return dtA == dtB;
}

bool cs::SGNode::SameScalarType(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return FloatType(dtA, dtB) || IntType(dtA, dtB);
}

bool cs::SGNode::SameTypeOrOne(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  if (FloatType(dtA, dtB))
  {
    return dtA == dtB || dtA == cs::eSGDT_Float || dtB == cs::eSGDT_Float;
  }
  if (IntType(dtA, dtB))
  {
    return dtA == dtB || dtA == cs::eSGDT_Int || dtB == cs::eSGDT_Int;
  }
  return false;
}


cs::eSGDataType cs::SGNode::HighOrderType(cs::eSGDataType dtA, cs::eSGDataType dtB) const
{
  return dtA > dtB ? dtA : dtB;
}

bool cs::SGNode::Validate()
{
  SetValidationMessage("");
  bool success = true;
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    cs::SGInput *input = m_inputs[i];
    if (!input)
    {
      continue;
    }
    cs::SGOutput *output = input->GetInput();
    if (!output)
    {
      continue;
    }
    cs::SGNode *node = output->GetNode();
    if (node)
    {
      if (node == this)
      {
        success = false;
        return false;
      }
      if (!node->Validate())
      {
        success = false;
      }
    }
  }

  return success;
}





