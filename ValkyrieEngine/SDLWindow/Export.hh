
#pragma once

#ifdef VK_WIN32

#ifdef VKSDL_EXPORT
#define VKSDL_API __declspec(dllexport)
#else
#define VKSDL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )


#else

#define VKSDL_API

#endif

