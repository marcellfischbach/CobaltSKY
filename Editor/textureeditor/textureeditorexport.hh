
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)



#ifdef TEXTUREEDITOR_EXPORT
#define TEXTUREEDITOR_API __declspec(dllexport)
#else
#define TEXTUREEDITOR_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define TEXTUREEDITOR_API

#endif

