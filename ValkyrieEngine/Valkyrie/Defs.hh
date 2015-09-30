#pragma once



#if VK_WIN32

#define VK_FORCEINLINE __forceinline

#else

#define VK_FORCEINLINE inline

#endif