
#pragma once

#ifdef VK_WIN32

#ifdef VKGL4_EXPORT
#define VKGL4_API __declspec(dllexport)
#else
#define VKGL4_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )
#pragma warning( disable : 4530)
#pragma warning( disable : 4577)


#else

#define VKGL4_API

#endif

