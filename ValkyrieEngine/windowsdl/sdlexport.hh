
#pragma once

#ifdef VK_WIN32

#ifdef VKWINDOWSDL_EXPORT
#define VKWINDOWSDL_API __declspec(dllexport)
#else
#define VKWINDOWSDL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )


#else

#define VKSDL_API

#endif

