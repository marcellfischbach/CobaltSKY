
#include <cobalt/graphics/shadergraph/cssgoutput.hh>


cs::SGOutput::SGOutput(cs::eSGDataType type, const std::string &name, const std::string &attr)
  : m_dataType(type)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

cs::SGOutput::SGOutput(const std::string &name, const std::string &attr)
  : m_dataType(cs::eSGDT_Inval)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

cs::SGOutput::~SGOutput()
{

}

