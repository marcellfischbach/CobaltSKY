
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)



#ifdef ASSET_MANAGER_EXPORT
#define ASSET_MANAGER_API __declspec(dllexport)
#else
#define ASSET_MANAGER_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define ASSET_MANAGER_API

#endif

