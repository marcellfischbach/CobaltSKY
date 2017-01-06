
#include <valkyrie/graphics/shadergraph/vksgnode.hh>




vkSGNode::vkSGNode()
  : vkObject()
{
}

vkSGNode::~vkSGNode()
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



void vkSGNode::AddInput(vkSGInput *input)
{
  if (input)
  {
    input->m_node = this;
    input->SetIdx((vkUInt32)m_inputs.size());
    m_inputs.push_back(input);
  }
}

vkSize vkSGNode::GetNumberOfInputs() const
{
  return m_inputs.size();
}

vkSGInput *vkSGNode::GetInput(vkSize idx) const
{
  if (idx >= m_inputs.size())
  {
    return 0;
  }
  return m_inputs[idx];
}

vkSGInput *vkSGNode::GetInput(const vkString &name) const
{
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    vkSGInput *input = m_inputs[i];
    if (name == input->GetName())
    {
      return input;
    }
  }
  return 0;
}

vkSGDataType vkSGNode::GetInputDataType(vkSize idx) const
{
  vkSGInput *input = GetInput(idx);
  if (!input)
  {
    return eSGDT_Inval;
  }
  return input->GetDataType();
}

vkSGDataType vkSGNode::GetInputDataType(const vkString &name) const
{
  vkSGInput *input = GetInput(name);
  if (!input)
  {
    return eSGDT_Inval;
  }
  return input->GetDataType();
}

void vkSGNode::SetOutputDataType(vkSize idx, vkSGDataType dataType)
{
  vkSGOutput *output = GetOutput(idx);
  if (output)
  {
    output->SetDataType(dataType);
  }
}


void vkSGNode::SetOutputDataType(const vkString &name, vkSGDataType dataType)
{
  vkSGOutput *output = GetOutput(name);
  if (output)
  {
    output->SetDataType(dataType);
  }
}


void vkSGNode::SetInput(int inputIdx, vkSGNode *node, int outputIdx)
{
  vkSGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputIdx);
  }

  input->SetInput(output);
}

void vkSGNode::SetInput(const vkString &inputName, vkSGNode *node, int outputIdx)
{
  vkSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputIdx);
  }

  input->SetInput(output);
}


void vkSGNode::SetInput(int inputIdx, vkSGNode *node, const vkString &outputName)
{
  vkSGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputName);
  }

  input->SetInput(output);
}

void vkSGNode::SetInput(const vkString &inputName, vkSGNode *node, const vkString &outputName)
{
  vkSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = 0;
  if (node)
  {
    output = node->GetOutput(outputName);
  }

  input->SetInput(output);
}

void vkSGNode::SetInput(int idx, float constFloat)
{
  vkSGInput *input = GetInput(idx);
  if (!input)
  {
    return;
  }

  input->SetConst(constFloat);
}

void vkSGNode::SetInput(const vkString &inputName, float constFloat)
{
  vkSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }

  input->SetConst(constFloat);
}

vkSGNode *vkSGNode::GetInputNode(vkSize idx)
{
  vkSGInput *input = GetInput(idx);
  return (input ? input->GetInputNode() : 0);
}

vkSGNode *vkSGNode::GetInputNode(const vkString &name)
{
  vkSGInput *input = GetInput(name);
  return input ? input->GetInputNode() : 0;
}

void vkSGNode::AddOutput(vkSGOutput *output)
{
  if (output)
  {
    output->m_node = this;
    output->SetIdx((vkUInt32)m_outputs.size());
    m_outputs.push_back(output);
  }
}

vkSize vkSGNode::GetNumberOfOutputs() const
{
  return m_outputs.size();
}

vkSGOutput *vkSGNode::GetOutput(vkSize idx)
{
  if (idx >= m_outputs.size())
  {
    return 0;
  }
  return m_outputs[idx];
}

vkSGOutput *vkSGNode::GetOutput(const vkString &name)
{
  for (size_t i = 0, in = m_outputs.size(); i < in; ++i)
  {
    vkSGOutput *output = m_outputs[i];
    if (name == output->GetName())
    {
      return output;
    }
  }
  return 0;
}

bool vkSGNode::NotInvalid(vkSGDataType dt) const
{
  return dt != eSGDT_Inval;
}

bool vkSGNode::ScalarType(vkSGDataType dt) const
{
  return (dt & eSGDT_ScalarTypes) == dt;
}

bool vkSGNode::FloatType(vkSGDataType dt) const
{
  return (dt & eSGDT_FloatTypes) == dt;
}

bool vkSGNode::IntType(vkSGDataType dt) const
{
  return (dt & eSGDT_IntTypes) == dt;
}

bool vkSGNode::NotInvalid(vkSGDataType dtA, vkSGDataType dtB) const
{
  return NotInvalid(dtA) && NotInvalid(dtB);
}

bool vkSGNode::ScalarType(vkSGDataType dtA, vkSGDataType dtB) const
{
  return ScalarType(dtA) && ScalarType(dtB);
}


bool vkSGNode::FloatType(vkSGDataType dtA, vkSGDataType dtB) const
{
  return FloatType(dtA) && FloatType(dtB);
}


bool vkSGNode::IntType(vkSGDataType dtA, vkSGDataType dtB) const
{
  return IntType(dtA) && IntType(dtB);
}

bool vkSGNode::SameType(vkSGDataType dtA, vkSGDataType dtB) const
{
  return dtA == dtB;
}

bool vkSGNode::SameScalarType(vkSGDataType dtA, vkSGDataType dtB) const
{
  return FloatType(dtA, dtB) || IntType(dtA, dtB);
}

bool vkSGNode::SameTypeOrOne(vkSGDataType dtA, vkSGDataType dtB) const
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


vkSGDataType vkSGNode::HighOrderType(vkSGDataType dtA, vkSGDataType dtB) const
{
  return dtA > dtB ? dtA : dtB;
}

bool vkSGNode::Validate()
{
  SetValidationMessage("");
  bool success = true;
  for (size_t i = 0, in = m_inputs.size(); i < in; ++i)
  {
    vkSGInput *input = m_inputs[i];
    if (!input)
    {
      continue;
    }
    vkSGOutput *output = input->GetInput();
    if (!output)
    {
      continue;
    }
    vkSGNode *node = output->GetNode();
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





