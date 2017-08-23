
#include <cobalt/graphics/shadergraph/cssgoutput.hh>


csSGOutput::csSGOutput(csSGDataType type, const std::string &name, const std::string &attr)
  : m_dataType(type)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

csSGOutput::csSGOutput(const std::string &name, const std::string &attr)
  : m_dataType(eSGDT_Inval)
  , m_name(name)
  , m_node(0)
  , m_attr(attr)
{

}

csSGOutput::~csSGOutput()
{

}

