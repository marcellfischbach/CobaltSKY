
#pragma once

#ifdef VK_WIN32

#ifdef VKE_EXPORT
#define VKE_API __declspec(dllexport)
#else
#define VKE_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define VKE_API

#endif

