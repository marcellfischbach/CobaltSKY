
#pragma once

#ifdef CS_WIN32

#ifdef CSWINDOWSDL_EXPORT
#define CSWINDOWSDL_API __declspec(dllexport)
#else
#define CSWINDOWSDL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )


#else

#define CSSDL_API

#endif

