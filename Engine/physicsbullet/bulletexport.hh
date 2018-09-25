
#pragma once

#ifdef CS_WIN32

#ifdef CSBULLETPHYSICS_EXPORT
#define CSBULLETPHYSICS_API __declspec(dllexport)
#else
#define CSBULLETPHYSICS_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CSBULLET_API

#endif

