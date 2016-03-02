
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>



vkSGOutput::vkSGOutput(vkSGDataType type, const vkString &name, const vkString &attr)
  : m_dataType(type)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

vkSGOutput::vkSGOutput(const vkString &name, const vkString &attr)
  : m_dataType(eSGDT_Inval)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

vkSGOutput::~vkSGOutput()
{

}




vkSGInput::vkSGInput(const vkString &name, bool canConst, bool canInputNode)
  : m_name(name)
  , m_node(0)
  , m_input(0)
  , m_constFloat(0.0)
  , m_canInputConst(canConst)
  , m_canInputNode(canInputNode)
{
}


vkSGInput::~vkSGInput()
{

}


vkSGNode *vkSGInput::GetInputNode()
{
  if (!m_input)
  {
    return 0;
  }
  return m_input->GetNode();
}

void vkSGInput::SetInput(vkSGOutput *input)
{
  if (m_canInputNode)
  {
    m_input = input;
  }
}

vkSGDataType vkSGInput::GetDataType() const
{
  if (m_canInputNode && m_input)
  {
    return m_input->GetDataType();
  }
  if (m_canInputConst)
  {
    return eSGDT_Float;
  }
  return eSGDT_Inval;
}


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
  if (!node)
  {
    return;
  }
  vkSGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = node->GetOutput(outputIdx);
  if (!output)
  {
    return;
  }

  input->SetInput(output);
}

void vkSGNode::SetInput(const vkString &inputName, vkSGNode *node, int outputIdx)
{
  if (!node)
  {
    return;
  }
  vkSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = node->GetOutput(outputIdx);
  if (!output)
  {
    return;
  }

  input->SetInput(output);
}


void vkSGNode::SetInput(int inputIdx, vkSGNode *node, const vkString &outputName)
{
  if (!node)
  {
    return;
  }
  vkSGInput *input = GetInput(inputIdx);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = node->GetOutput(outputName);
  if (!output)
  {
    return;
  }

  input->SetInput(output);
}

void vkSGNode::SetInput(const vkString &inputName, vkSGNode *node, const vkString &outputName)
{
  if (!node)
  {
    return;
  }
  vkSGInput *input = GetInput(inputName);
  if (!input)
  {
    return;
  }
  vkSGOutput *output = node->GetOutput(outputName);
  if (!output)
  {
    return;
  }

  input->SetInput(output);
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
        printf("Bound to itself.\n");
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

vkSGVarFloat::vkSGVarFloat()
  : vkSGNode()
{
  SetName("Var/Float");
  AddOutput(new vkSGOutput(eSGDT_Float, "v"));
}

vkSGVarFloat2::vkSGVarFloat2()
  : vkSGNode()
{
  SetName("Var/Float2");
  AddOutput(new vkSGOutput(eSGDT_Float2, "v"));
}

vkSGVarFloat3::vkSGVarFloat3()
  : vkSGNode()
{
  SetName("Var/Float3");
  AddOutput(new vkSGOutput(eSGDT_Float3, "v"));
}

vkSGVarFloat4::vkSGVarFloat4()
  : vkSGNode()
{
  SetName("Var/Float4");
  AddOutput(new vkSGOutput(eSGDT_Float4, "v"));
}

vkSGConstFloat::vkSGConstFloat()
  : vkSGNode()
{
  SetName("Const/Float");
  AddInput(new vkSGInput("x", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float, "v", ""));
}

vkSGConstFloat2::vkSGConstFloat2()
  : vkSGNode()
{
  SetName("Const/Float2");
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float2, "v", ""));
}

vkSGConstFloat3::vkSGConstFloat3()
  : vkSGNode()
{
  SetName("Const/Float3");
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddInput(new vkSGInput("z", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
}

vkSGConstFloat4::vkSGConstFloat4()
  : vkSGNode()
{
  SetName("Const/Float4");
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddInput(new vkSGInput("z", true, false));
  AddInput(new vkSGInput("w", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float4, "v", ""));
}


vkSGFloat2::vkSGFloat2()
  : vkSGNode()
{
  SetName("Assemble/Float2");
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float2, "v", ""));
}


vkSGAdd::vkSGAdd()
  : vkSGNode()
{
  SetName("Math/Add");
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput("v"));
}

bool vkSGAdd::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (!ScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid mix of int/float");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}

vkSGSub::vkSGSub()
  : vkSGNode()
{
  SetName("Math/Sub");
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGSub::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (!ScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameTypeOrOne(dtA, dtB))
  {
    SetValidationMessage("Invalid mix of int/float");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}

vkSGDefaultTextureCoordinate::vkSGDefaultTextureCoordinate()
  : vkSGNode()
{
  AddOutput(new vkSGOutput(eSGDT_Float2, "uv"));
}

vkSGTexture2D::vkSGTexture2D()
  : vkSGNode()
{
  AddInput(new vkSGInput("uv", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float4, "c"));
  AddOutput(new vkSGOutput(eSGDT_Float, "r", "r"));
  AddOutput(new vkSGOutput(eSGDT_Float, "g", "g"));
  AddOutput(new vkSGOutput(eSGDT_Float, "b", "b"));
  AddOutput(new vkSGOutput(eSGDT_Float, "a", "a"));
}