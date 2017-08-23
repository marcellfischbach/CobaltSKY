
#include <cobalt/graphics/shadergraph/cssgnode.hh>




csSGNode::csSGNode()
  : csObject()
{
}

csSGNode::~csSGNode()
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

csSGNode *csSGNode::Copy(csSGNode *node) const
{
  node->m_name = m_name;
  return node;
}

void csSGNode::AddInput(csSGInput *input)
{
  if (input)
  {
    input->m_node = this;
    input->SetIdx((csUInt32)m_inputs.size());
    m_inputs.push_back(input);
  }
}

csSize csSGNode::GetNumberOfInputs() const
{
  return m_inputs.size();
}

csSGInput *csSGNode::GetInput(csSize idx) const
{
  if (idx >= m_inputs.size())
  {
    return 0;
  }
  return m_inputs[idx];
}

csSGInput *csSGNode::GetInput(const std::string &name) const
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    csSGInput *input = m_inputs[i];
    if (name == input->GetName())
    {
      return input;
    }
  }
  return 0;
}

csSGDataType csSGNode::GetInputDataType(csSize idx) const
{
  csSGInput *input = GetInput(idx);
  if (!input)
  {
    return eSGDT_Inval;
  }
  return input->GetDataType();
}

csSGDataType csSGNode::GetInputDataType(const std::string &name) const
{
  csSGInput *input = GetInput(name);
  if (!input)
  {
    return eSGDT_Inval;
  }
  return input->GetDataType();
}

void csSGNode::SetOutputDataType(csSize idx, csSGDataType dataType)
{
  csSGOutput *output = GetOutput(idx);
  if (output)
  {
    output->SetDataType(dataType);
  }
}


void csSGNode::SetOutputDataType(const std::string &name, csSGDataType dataType)
{
  csSGOutput *output = GetOutput(name);
  if (output)
  {
    output->SetDataType(dataType);
  }
}


void csSGNode::SetInput(int inputIdx, csSGNode *node, int outputIdx)
{
  csSGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  csSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputIdx);
  }

  input->SetInput(output);
}

void csSGNode::SetInput(const std::string &inputName, csSGNode *node, int outputIdx)
{
  csSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  csSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputIdx);
  }

  input->SetInput(output);
}


void csSGNode::SetInput(int inputIdx, csSGNode *node, const std::string &outputName)
{
  csSGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  csSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputName);
  }

  input->SetInput(output);
}

void csSGNode::SetInput(const std::string &inputName, csSGNode *node, const std::string &outputName)
{
  csSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  csSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputName);
  }

  input->SetInput(output);
}

void csSGNode::SetInput(int idx, float constFloat)
{
  csSGInput *input = GetInput(idx);
  if (!input)
  {
    return;
  }

  input->SetConst(constFloat);
}

void csSGNode::SetInput(const std::string &inputName, float constFloat)
{
  csSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }

  input->SetConst(constFloat);
}

csSGNode *csSGNode::GetInputNode(csSize idx)
{
  csSGInput *input = GetInput(idx);
  return (input ? input->GetInputNode() : 0);
}

csSGNode *csSGNode::GetInputNode(const std::string &name)
{
  csSGInput *input = GetInput(name);
  return input ? input->GetInputNode() : 0;
}

void csSGNode::AddOutput(csSGOutput *output)
{
  if (output)
  {
    output->m_node = this;
    output->SetIdx((csUInt32)m_outputs.size());
    m_outputs.push_back(output);
  }
}

csSize csSGNode::GetNumberOfOutputs() const
{
  return m_outputs.size();
}

csSGOutput *csSGNode::GetOutput(csSize idx)
{
  if (idx >= m_outputs.size())
  {
    return 0;
  }
  return m_outputs[idx];
}

csSGOutput *csSGNode::GetOutput(const std::string &name)
{
  for (size_t i = 0, in = m_outputs.size(); i < in; ++i)
  {
    csSGOutput *output = m_outputs[i];
    if (name == output->GetName())
    {
      return output;
    }
  }
  return 0;
}

bool csSGNode::NotInvalid(csSGDataType dt) const
{
  return dt != eSGDT_Inval;
}

bool csSGNode::ScalarType(csSGDataType dt) const
{
  return (dt & eSGDT_ScalarTypes) == dt;
}

bool csSGNode::FloatType(csSGDataType dt) const
{
  return (dt & eSGDT_FloatTypes) == dt;
}

bool csSGNode::IntType(csSGDataType dt) const
{
  return (dt & eSGDT_IntTypes) == dt;
}

bool csSGNode::NotInvalid(csSGDataType dtA, csSGDataType dtB) const
{
  return NotInvalid(dtA) && NotInvalid(dtB);
}

bool csSGNode::ScalarType(csSGDataType dtA, csSGDataType dtB) const
{
  return ScalarType(dtA) && ScalarType(dtB);
}


bool csSGNode::FloatType(csSGDataType dtA, csSGDataType dtB) const
{
  return FloatType(dtA) && FloatType(dtB);
}


bool csSGNode::IntType(csSGDataType dtA, csSGDataType dtB) const
{
  return IntType(dtA) && IntType(dtB);
}

bool csSGNode::SameType(csSGDataType dtA, csSGDataType dtB) const
{
  return dtA == dtB;
}

bool csSGNode::SameScalarType(csSGDataType dtA, csSGDataType dtB) const
{
  return FloatType(dtA, dtB) || IntType(dtA, dtB);
}

bool csSGNode::SameTypeOrOne(csSGDataType dtA, csSGDataType dtB) const
{
  if (FloatType(dtA, dtB))
  {
    return dtA == dtB || dtA == eSGDT_Float || dtB == eSGDT_Float;
  }
  if (IntType(dtA, dtB))
  {
    return dtA == dtB || dtA == eSGDT_Int || dtB == eSGDT_Int;
  }
  return false;
}


csSGDataType csSGNode::HighOrderType(csSGDataType dtA, csSGDataType dtB) const
{
  return dtA > dtB ? dtA : dtB;
}

bool csSGNode::Validate()
{
  SetValidationMessage("");
  bool success = true;
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    csSGInput *input = m_inputs[i];
    if (!input)
    {
      continue;
    }
    csSGOutput *output = input->GetInput();
    if (!output)
    {
      continue;
    }
    csSGNode *node = output->GetNode();
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





