
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>



cs::SGInput::SGInput(const std::string &name, bool canConst, bool canInputNode)
  : m_name(name)
  , m_node(0)
  , m_input(0)
  , m_constFloat(0.0)
  , m_canInputConst(canConst)
  , m_canInputNode(canInputNode)
{
}


cs::SGInput::~SGInput()
{

}


cs::SGNode *cs::SGInput::GetInputNode()
{
  if (!m_input)
  {
    return 0;
  }
  return m_input->GetNode();
}

void cs::SGInput::SetInput(cs::SGOutput *input)
{
  if (m_canInputNode)
  {
    m_input = input;
  }
}

cs::eSGDataType cs::SGInput::GetDataType() const
{
  if (m_canInputNode && m_input)
  {
    return m_input->GetDataType();
  }
  if (m_canInputConst)
  {
    return cs::eSGDT_Float;
  }
  return cs::eSGDT_Inval;
}
