#pragma once


#define VK_FLOAT_IS_ZERO(e) (e < 1e-6 && e > -1e-6)
#define VK_FLOAT_IS_ONE(e) (e < (1.0 + 1e-6) && e > (1.0 - 1e-6))
#define VK_FLOAT_IS(e, v) (e < (v + 1e-6) && e > (v - 1e-6))

#define VK_VERSION(maj,min,mic) ((((maj) & 0xff) << 24) | (((min) & 0xff) << 16) | ((mic) & 0xffff))

#if VK_WIN32


#define VK_FORCEINLINE __forceinline

#define VK_ZERO(e) memset(&e, 0, sizeof(e))

#else

#define VK_FORCEINLINE inline

#endif