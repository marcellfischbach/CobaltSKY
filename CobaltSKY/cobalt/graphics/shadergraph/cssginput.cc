
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>



csSGInput::csSGInput(const csString &name, bool canConst, bool canInputNode)
  : m_name(name)
  , m_node(0)
  , m_input(0)
  , m_constFloat(0.0)
  , m_canInputConst(canConst)
  , m_canInputNode(canInputNode)
{
}


csSGInput::~csSGInput()
{

}


csSGNode *csSGInput::GetInputNode()
{
  if (!m_input)
  {
    return 0;
  }
  return m_input->GetNode();
}

void csSGInput::SetInput(csSGOutput *input)
{
  if (m_canInputNode)
  {
    m_input = input;
  }
}

csSGDataType csSGInput::GetDataType() const
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
