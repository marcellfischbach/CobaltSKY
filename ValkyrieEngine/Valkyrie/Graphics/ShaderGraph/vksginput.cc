
#include <Valkyrie/Graphics/ShaderGraph/vksginput.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgoutput.hh>



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
