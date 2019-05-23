
#include <cobalt/graphics/csnosuchparameterexception.hh>


cs::NoSuchParameterNameException::NoSuchParameterNameException(const std::string &name)
  : std::exception()
  , m_name(name)
{

}


cs::NoSuchParameterIdException::NoSuchParameterIdException(const std::string &id)
  : std::exception()
  , m_id(id)
{

}

cs::NoSuchParameterIndexException::NoSuchParameterIndexException(csSize idx)
  : std::exception()
  , m_idx(idx)
{

}
