
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef CSREFL_EXPORT
#define CSREFL_API __declspec(dllexport)
#else
#define CSREFL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CSREFL_API

#endif

