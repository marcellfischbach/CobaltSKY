
#include <Valkyrie/Graphics/shadergraph/vksgoutput.hh>


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

