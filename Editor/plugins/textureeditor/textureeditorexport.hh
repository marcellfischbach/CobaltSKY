
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)



#ifdef TEXTURE_EDITOR_EXPORT
#define TEXTURE_EDITOR_API __declspec(dllexport)
#else
#define TEXTURE_EDITOR_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define TEXTURE_EDITOR_API

#endif

