#pragma once

#include <cobalt/csexport.hh>
#include <exception>
#include <string>


class CSE_API csException : public std::exception
{
public:
    csException ();
    csException (const std::string &what);
    virtual ~csException ();
    virtual const char *what () const noexcept
    {
        return m_what.c_str();
    }

private:
    std::string m_what;
};

