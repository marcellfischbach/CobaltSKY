
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef MATERIALEDITOR_EXPORT
#define MATERIALEDITOR_API __declspec(dllexport)
#else
#define MATERIALEDITOR_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define MATERIALEDITOR_API

#endif

