
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

#define VK_VAR_FLOAT_NAME "Var/Float"
#define VK_VAR_FLOAT2_NAME "Var/Float2"
#define VK_VAR_FLOAT3_NAME "Var/Float3"
#define VK_VAR_FLOAT4_NAME "Var/Float4"

#define VK_CONST_FLOAT_NAME "Const/Float"
#define VK_CONST_FLOAT2_NAME "Const/Float2"
#define VK_CONST_FLOAT3_NAME "Const/Float3"
#define VK_CONST_FLOAT4_NAME "Const/Float4"

#define VK_FLOAT2_NAME "Assemble/Float2"
#define VK_FLOAT3_NAME "Assemble/Float3"
#define VK_FLOAT4_NAME "Assemble/Float4"

#define VK_SPLIT_FLOAT2_NAME "Assemble/SplitFloat2"
#define VK_SPLIT_FLOAT3_NAME "Assemble/SplitFloat3"
#define VK_SPLIT_FLOAT4_NAME "Assemble/SplitFloat4"

#define VK_ADD_NAME "Math/Add"
#define VK_SUB_NAME "Math/Sub"
#define VK_MUL_NAME "Math/Mul"
#define VK_DIV_NAME "Math/Div"
#define VK_DOT_NAME "Math/Dot"
#define VK_CROSS_NAME "Math/Cross"
#define VK_LERP_NAME "Math/Lerp"


#define VK_DEFAULT_TEXTURE_COORDINATE_NAME "Texture/DefaultTextureCoordinate"
#define VK_TEXTURE2D_NAME "Texture/Texture2D"

vkSGNodes::Entry::Entry(const vkString &name, const vkClass *clazz)
  : name(name)
  , clazz(clazz)
{

}

vkSGNodes::vkSGNodes()
{
  m_entries.push_back(Entry (VK_VAR_FLOAT_NAME, vkSGVarFloat::GetStaticClass()));
  m_entries.push_back(Entry (VK_VAR_FLOAT2_NAME, vkSGVarFloat2::GetStaticClass()));
  m_entries.push_back(Entry (VK_VAR_FLOAT3_NAME, vkSGVarFloat3::GetStaticClass()));
  m_entries.push_back(Entry (VK_VAR_FLOAT4_NAME, vkSGVarFloat4::GetStaticClass()));

  m_entries.push_back(Entry (VK_CONST_FLOAT_NAME, vkSGConstFloat::GetStaticClass()));
  m_entries.push_back(Entry (VK_CONST_FLOAT2_NAME, vkSGConstFloat2::GetStaticClass()));
  m_entries.push_back(Entry (VK_CONST_FLOAT3_NAME, vkSGConstFloat3::GetStaticClass()));
  m_entries.push_back(Entry (VK_CONST_FLOAT4_NAME, vkSGConstFloat4::GetStaticClass()));

  m_entries.push_back(Entry (VK_FLOAT2_NAME, vkSGFloat2::GetStaticClass()));
  m_entries.push_back(Entry (VK_FLOAT3_NAME, vkSGFloat3::GetStaticClass()));
  m_entries.push_back(Entry (VK_FLOAT4_NAME, vkSGFloat4::GetStaticClass()));

  m_entries.push_back(Entry (VK_SPLIT_FLOAT2_NAME, vkSGSplitFloat2::GetStaticClass()));
  m_entries.push_back(Entry (VK_SPLIT_FLOAT3_NAME, vkSGSplitFloat3::GetStaticClass()));
  m_entries.push_back(Entry (VK_SPLIT_FLOAT4_NAME, vkSGSplitFloat4::GetStaticClass()));

  m_entries.push_back(Entry (VK_ADD_NAME, vkSGAdd::GetStaticClass()));
  m_entries.push_back(Entry (VK_SUB_NAME, vkSGSub::GetStaticClass()));
  m_entries.push_back(Entry(VK_MUL_NAME, vkSGMul::GetStaticClass()));
  m_entries.push_back(Entry(VK_DIV_NAME, vkSGDiv::GetStaticClass()));
  m_entries.push_back(Entry(VK_DOT_NAME, vkSGDot::GetStaticClass()));
  m_entries.push_back(Entry(VK_CROSS_NAME, vkSGCross::GetStaticClass()));
  m_entries.push_back(Entry(VK_LERP_NAME, vkSGLerp::GetStaticClass()));

  m_entries.push_back(Entry (VK_DEFAULT_TEXTURE_COORDINATE_NAME, vkSGDefaultTextureCoordinate::GetStaticClass()));
  m_entries.push_back(Entry (VK_TEXTURE2D_NAME, vkSGTexture2D::GetStaticClass()));
}

vkSGNodes *vkSGNodes::Get()
{
  static vkSGNodes static_instance;
  return &static_instance;
}

const std::vector<vkSGNodes::Entry> &vkSGNodes::GetEntries () const
{
  return m_entries;
}

vkSGResourceNode::vkSGResourceNode()
  : vkSGNode()
  , m_resourceName("")
{

}

vkSGResourceNode::~vkSGResourceNode()
{

}

void vkSGResourceNode::SetResourceName(const vkString &resourceName)
{
  m_resourceName = resourceName;
}

const vkString &vkSGResourceNode::GetResourceName() const
{
  return m_resourceName;
}

void vkSGResourceNode::SetResourceType(vkShaderParameterType resourceType)
{
  m_resourceType = resourceType;
}

vkShaderParameterType vkSGResourceNode::GetResourceType() const
{
  return m_resourceType;
}

float *vkSGResourceNode::GetDefaultFloats()
{
  return m_defaultFloats;
}

int *vkSGResourceNode::GetDefaultInts()
{
  return m_defaultInts;
}

vkResourceLocator &vkSGResourceNode::GetDefaultTextureResource()
{
  return m_defaultTextureResource;
}

const float *vkSGResourceNode::GetDefaultFloats() const
{
  return m_defaultFloats;
}

const int *vkSGResourceNode::GetDefaultInts() const
{
  return m_defaultInts;
}

const vkResourceLocator &vkSGResourceNode::GetDefaultTextureResource() const
{
  return m_defaultTextureResource;
}

vkSGVarFloat::vkSGVarFloat()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT_NAME);
  SetResourceType(eSPT_Float);
  AddOutput(new vkSGOutput(eSGDT_Float, "v"));
}

vkSGVarFloat2::vkSGVarFloat2()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT2_NAME);
  SetResourceType(eSPT_Vector2);
  AddOutput(new vkSGOutput(eSGDT_Float2, "v"));
}

vkSGVarFloat3::vkSGVarFloat3()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT3_NAME);
  SetResourceType(eSPT_Vector3);
  AddOutput(new vkSGOutput(eSGDT_Float3, "v"));
}

vkSGVarFloat4::vkSGVarFloat4()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT4_NAME);
  SetResourceType(eSPT_Vector4);
  AddOutput(new vkSGOutput(eSGDT_Float4, "v"));
}

vkSGConstFloat::vkSGConstFloat()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float, "v", ""));
}

vkSGConstFloat2::vkSGConstFloat2()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT2_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float2, "v", ""));
}

vkSGConstFloat3::vkSGConstFloat3()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT3_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddInput(new vkSGInput("z", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
}

vkSGConstFloat4::vkSGConstFloat4()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT4_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddInput(new vkSGInput("z", true, false));
  AddInput(new vkSGInput("w", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float4, "v", ""));
}


vkSGFloat2::vkSGFloat2()
  : vkSGNode()
{
  SetName(VK_FLOAT2_NAME);
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float2, "v", ""));
}

vkSGFloat3::vkSGFloat3()
  : vkSGNode()
{
  SetName(VK_FLOAT3_NAME);
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddInput(new vkSGInput("z", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v", ""));
}


vkSGFloat4::vkSGFloat4()
  : vkSGNode()
{
  SetName(VK_FLOAT4_NAME);
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddInput(new vkSGInput("z", true, true));
  AddInput(new vkSGInput("w", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float4, "v", ""));
}

vkSGSplitFloat2::vkSGSplitFloat2()
  : vkSGNode()
{
  SetName(VK_SPLIT_FLOAT2_NAME);
  AddInput(new vkSGInput("v", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
}

bool vkSGSplitFloat2::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float2;
}


vkSGSplitFloat3::vkSGSplitFloat3()
  : vkSGNode()
{
  SetName(VK_SPLIT_FLOAT3_NAME);
  AddInput(new vkSGInput("v", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new vkSGOutput(eSGDT_Float, "z", "z"));
}

bool vkSGSplitFloat3::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float3;
}


vkSGSplitFloat4::vkSGSplitFloat4()
  : vkSGNode()
{
  SetName(VK_SPLIT_FLOAT4_NAME);
  AddInput(new vkSGInput("v", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "x", "x"));
  AddOutput(new vkSGOutput(eSGDT_Float, "y", "y"));
  AddOutput(new vkSGOutput(eSGDT_Float, "z", "z"));
  AddOutput(new vkSGOutput(eSGDT_Float, "w", "w"));
}

bool vkSGSplitFloat4::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dt = GetInputDataType(0);

  return success && dt == eSGDT_Float4;
}

vkSGAdd::vkSGAdd()
  : vkSGNode()
{
  SetName(VK_ADD_NAME);
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
  if (!SameTypeOrOne(dtA, dtB))
  {
    SetValidationMessage("Invalid combination of types in add");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}

vkSGSub::vkSGSub()
  : vkSGNode()
{
  SetName(VK_SUB_NAME);
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
  if (!SameScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid mix of int/float");
    return false;
  }
  if (!SameTypeOrOne(dtA, dtB))
  {
    SetValidationMessage("Invalid combination of types in sub");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}

vkSGMul::vkSGMul()
  : vkSGNode()
{
  SetName(VK_MUL_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGMul::Validate()
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
  if (!SameTypeOrOne(dtA, dtB))
  {
    SetValidationMessage("Invalid combination of types in mul");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}

vkSGDiv::vkSGDiv()
  : vkSGNode()
{
  SetName(VK_DIV_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGDiv::Validate()
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

  if (!SameTypeOrOne(dtA, dtB))
  {
    SetValidationMessage("Invalid combination of types in div");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}


vkSGDot::vkSGDot()
  : vkSGNode()
{
  SetName(VK_DOT_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float, "v"));
}


bool vkSGDot::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (!ScalarType(dtA, dtB))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameType(dtA, dtB))
  {
    SetValidationMessage("Invalid mix of types");
    return false;
  }

  SetOutputDataType(0, eSGDT_Float);

  return success;
}


vkSGCross::vkSGCross()
  : vkSGNode()
{
  SetName(VK_CROSS_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float3, "v"));
}


bool vkSGCross::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  if (dtA != eSGDT_Float3 || dtB != eSGDT_Float3)
  {
    SetValidationMessage("Only Float3 supported for inputs");
    return false;
  }

  return success;
}



vkSGLerp::vkSGLerp()
  : vkSGNode()
{
  SetName(VK_LERP_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddInput(new vkSGInput("b", true, true));
  AddInput(new vkSGInput("x", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGLerp::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);
  vkSGDataType dtB = GetInputDataType(1);
  vkSGDataType dtX = GetInputDataType(2);
  if (!ScalarType(dtA, dtB) || !ScalarType(dtX))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  if (!SameScalarType(dtA, dtB) || !(SameScalarType(dtA, dtX) || dtX == eSGDT_Float)) 
  {
    SetValidationMessage("Invalid mix of int/float");
    return false;
  }
  if (!SameTypeOrOne(dtA, dtB) || !SameTypeOrOne(dtA, dtX))
  {
    SetValidationMessage("Invalid combination of types in mul");
    return false;
  }

  SetOutputDataType(0, HighOrderType(dtA, dtB));
  return success;
}



vkSGDefaultTextureCoordinate::vkSGDefaultTextureCoordinate()
  : vkSGNode()
{
  SetName(VK_DEFAULT_TEXTURE_COORDINATE_NAME);
  AddOutput(new vkSGOutput(eSGDT_Float2, "uv"));
}

vkSGTexture2D::vkSGTexture2D()
  : vkSGResourceNode()
{
  SetName(VK_TEXTURE2D_NAME);
  SetResourceType(eSPT_Texture);

  AddInput(new vkSGInput("uv", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float4, "c"));
  AddOutput(new vkSGOutput(eSGDT_Float, "r", "r"));
  AddOutput(new vkSGOutput(eSGDT_Float, "g", "g"));
  AddOutput(new vkSGOutput(eSGDT_Float, "b", "b"));
  AddOutput(new vkSGOutput(eSGDT_Float, "a", "a"));
}
