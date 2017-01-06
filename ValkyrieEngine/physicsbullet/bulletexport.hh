
#pragma once

#ifdef VK_WIN32

#ifdef VKBULLETPHYSICS_EXPORT
#define VKBULLETPHYSICS_API __declspec(dllexport)
#else
#define VKBULLETPHYSICS_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define VKBULLET_API

#endif

