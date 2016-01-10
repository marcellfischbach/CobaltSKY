
#pragma once

#ifdef VK_WIN32

#ifdef VKBULLET_EXPORT
#define VKBULLET_API __declspec(dllexport)
#else
#define VKBULLET_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define VKBULLET_API

#endif

