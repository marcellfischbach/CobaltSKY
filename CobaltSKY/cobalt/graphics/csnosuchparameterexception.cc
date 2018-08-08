
#include <cobalt/graphics/csnosuchparameterexception.hh>


csNoSuchParameterNameException::csNoSuchParameterNameException(const std::string &name)
  : std::exception()
  , m_name(name)
{

}


csNoSuchParameterIdException::csNoSuchParameterIdException(const std::string &id)
  : std::exception()
  , m_id(id)
{

}

csNoSuchParameterIndexException::csNoSuchParameterIndexException(csSize idx)
  : std::exception()
  , m_idx(idx)
{

}
