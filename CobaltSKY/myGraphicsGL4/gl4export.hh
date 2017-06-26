
#pragma once

#ifdef CS_WIN32

#ifdef CSGRAPHICSGL4_EXPORT
#define CSGRAPHICSGL4_API __declspec(dllexport)
#else
#define CSGRAPHICSGL4_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )
#pragma warning( disable : 4530)
#pragma warning( disable : 4577)


#else

#define CSGRAPHICSGL4_API

#endif

