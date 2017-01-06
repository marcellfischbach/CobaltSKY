
#pragma once

#ifdef VK_WIN32

#ifdef VKGRAPHICSGL4_EXPORT
#define VKGRAPHICSGL4_API __declspec(dllexport)
#else
#define VKGRAPHICSGL4_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )
#pragma warning( disable : 4530)
#pragma warning( disable : 4577)


#else

#define VKGRAPHICSGL4_API

#endif

