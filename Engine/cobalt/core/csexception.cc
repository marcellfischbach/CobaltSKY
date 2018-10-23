
#include <cobalt/core/csexception.hh>

csException::csException()
    : std::exception()
    , m_what("")
{

}

csException::csException(const std::string &what)
    : std::exception()
    , m_what(what)
{

}


csException::~csException()
{

}

