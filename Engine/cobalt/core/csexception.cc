
#include <cobalt/core/csexception.hh>

cs::Exception::Exception()
    : std::exception()
    , m_what("")
{

}

cs::Exception::Exception(const std::string &what)
    : std::exception()
    , m_what(what)
{

}


cs::Exception::~Exception()
{

}

