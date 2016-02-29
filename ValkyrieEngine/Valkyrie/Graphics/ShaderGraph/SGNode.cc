
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>



vkSGOutput::vkSGOutput(vkSGDataType type, const vkString &name, const vkString &attr)
  : m_dataType(type)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

vkSGOutput::~vkSGOutput()
{

}




vkSGInput::vkSGInput(vkUInt32 types, const vkString &name, bool canConst, bool canInputNode)
  : m_dataTypes(types)
  , m_name(name)
  , m_node(0)
  , m_input(0)
  , m_canConstData(canConst && (types == eSGDT_Float || types == eSGDT_Int))
  , m_canInputNode(canInputNode)
{
  memset(&m_constData, 0, sizeof(m_constData));
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


void vkSGInput::SetConstDataPossible(bool constDataPossible)
{
  m_canConstData = constDataPossible;
}

void vkSGInput::SetConstData(unsigned i0)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Int) != 0)
  {
    m_constDataType = eSGDT_Int;
    m_constData.u4[0] = i0;
  }
}


void vkSGInput::SetConstData(unsigned i0, unsigned i1)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Int2) != 0)
  {
    m_constDataType = eSGDT_Int2;
    m_constData.u4[0] = i0;
    m_constData.u4[1] = i1;
  }
}

void vkSGInput::SetConstData(unsigned i0, unsigned i1, unsigned i2)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Int3) != 0)
  {
    m_constDataType = eSGDT_Int3;
    m_constData.u4[0] = i0;
    m_constData.u4[1] = i1;
    m_constData.u4[2] = i2;
  }
}

void vkSGInput::SetConstData(unsigned i0, unsigned i1, unsigned i2, unsigned i3)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Int4) != 0)
  {
    m_constDataType = eSGDT_Int4;
    m_constData.u4[0] = i0;
    m_constData.u4[1] = i1;
    m_constData.u4[2] = i2;
    m_constData.u4[3] = i3;
  }
}


void vkSGInput::SetConstData(float i0)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Float) != 0)
  {
    m_constDataType = eSGDT_Float;
    m_constData.f4[0] = i0;
  }
}


void vkSGInput::SetConstData(float i0, float i1)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Float2) != 0)
  {
    m_constDataType = eSGDT_Float2;
    m_constData.f4[0] = i0;
    m_constData.f4[1] = i1;
  }
}

void vkSGInput::SetConstData(float i0, float i1, float i2)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Float3) != 0)
  {
    m_constDataType = eSGDT_Float3;
    m_constData.f4[0] = i0;
    m_constData.f4[1] = i1;
    m_constData.f4[2] = i2;
  }
}

void vkSGInput::SetConstData(float i0, float i1, float i2, float i3)
{
  if (m_canConstData && (m_dataTypes & eSGDT_Float4) != 0)
  {
    m_constDataType = eSGDT_Float4;
    m_constData.f4[0] = i0;
    m_constData.f4[1] = i1;
    m_constData.f4[2] = i2;
    m_constData.f4[3] = i3;
  }
}

vkSGNode::vkSGNode(vkSGNodeType type)
  : vkObject()
  , m_type(type)
{

  switch (type)
  {
  case eSGNT_ConstFloat3:
    SetName("ConstFloat3");
    AddInput(new vkSGInput(eSGDT_Float, "x", true, false));
    AddInput(new vkSGInput(eSGDT_Float, "y", true, false));
    AddInput(new vkSGInput(eSGDT_Float, "z", true, false));
    AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
    break;
  case eSGNT_Float3:
    SetName("Float3");
    AddInput(new vkSGInput(eSGDT_Float, "x", false, true));
    AddInput(new vkSGInput(eSGDT_Float, "y", false, true));
    AddInput(new vkSGInput(eSGDT_Float, "z", false, true));
    AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
    break;

  case eSGNT_SplitFloat3:
    SetName("SplitFloat3");
    AddInput(new vkSGInput(eSGDT_Float3, "v", false, true));
    AddOutput(new vkSGOutput(eSGDT_Float3, "x", "x"));
    AddOutput(new vkSGOutput(eSGDT_Float3, "y", "y"));
    AddOutput(new vkSGOutput(eSGDT_Float3, "z", "z"));
    break;

  case eSGNT_AddFloat3:
    SetName("AddFloat3");
    AddInput(new vkSGInput(eSGDT_Float3, "a", false, true));
    AddInput(new vkSGInput(eSGDT_Float3, "b", false, true));
    AddOutput(new vkSGOutput(eSGDT_Float3, "o", ""));
    break;

  }

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

void vkSGNode::SetName(const vkString &name)
{
  m_name = name;
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

vkSGInput *vkSGNode::GetInput(vkSize idx)
{
  if (idx >= m_inputs.size())
  {
    return 0;
  }
  return m_inputs[idx];
}

vkSGInput *vkSGNode::GetInput(const vkString &name)
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


/*



vkSGConstFloat::vkSGConstFloat()
  : vkSGNode()
  , m_value(0.0)
{
  SetName("ConstFloat");
  AddOutput(new vkSGOutput(eSGDT_Float, "Val", ""));
}


vkSGConstFloat::~vkSGConstFloat()
{

}


vkSGConstFloat3::vkSGConstFloat3()
  : vkSGNode()
  , m_valueX(0.0)
  , m_valueY(0.0)
  , m_valueZ(0.0)
{
  SetName("ConstFloat3");
  AddOutput(new vkSGOutput(eSGDT_Float3, "Val", ""));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "z", "x"));
}


vkSGConstFloat3::~vkSGConstFloat3()
{

}



vkSGFloat3::vkSGFloat3()
  : vkSGNode()
{
  SetName("Float3");
  AddOutput(new vkSGOutput(eSGDT_Float3, "Val", ""));
  AddInput(new vkSGInput(eSGDT_Float, "x"));
  AddInput(new vkSGInput(eSGDT_Float, "y"));
  AddInput(new vkSGInput(eSGDT_Float, "z"));
}


vkSGFloat3::~vkSGFloat3()
{

}



vkSGAddFloat::vkSGAddFloat()
  : vkSGNode()
{
  SetName("ADD");
  AddInput(new vkSGInput(eSGDT_Float, "A"));
  AddInput(new vkSGInput(eSGDT_Float, "B"));
  AddOutput(new vkSGOutput(eSGDT_Float, "Val", ""));
}

vkSGAddFloat::~vkSGAddFloat()
{

}



vkSGAddFloat3::vkSGAddFloat3()
  : vkSGNode()
{
  SetName("ADD3");
  AddInput(new vkSGInput(eSGDT_Float3, "A"));
  AddInput(new vkSGInput(eSGDT_Float3, "B"));
  AddOutput(new vkSGOutput(eSGDT_Float3, "Val", ""));
}

vkSGAddFloat3::~vkSGAddFloat3()
{

}



vkSGSplitFloat3::vkSGSplitFloat3()
  : vkSGNode()
{
  SetName("SplitFloat3");
  AddInput(new vkSGInput(eSGDT_Float3, "Val"));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new vkSGOutput(eSGDT_Float, "z", "z"));
}


vkSGSplitFloat3::~vkSGSplitFloat3()
{

}

*/

