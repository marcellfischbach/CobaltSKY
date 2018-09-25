
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef SHADERGRAPHEDITOR_EXPORT
#define SHADERGRAPHEDITOR_API __declspec(dllexport)
#else
#define SHADERGRAPHEDITOR_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SHADERGRAPHEDITOR_API

#endif

