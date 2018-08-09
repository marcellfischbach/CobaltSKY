#pragma once


#define CS_FLOAT_IS_ZERO(e) (e < 1e-6 && e > -1e-6)
#define CS_FLOAT_IS_ONE(e) (e < (1.0 + 1e-6) && e > (1.0 - 1e-6))
#define CS_FLOAT_IS(e, v) (e < (v + 1e-6) && e > (v - 1e-6))

#define CS_VERSION(maj,min,mic) ((((maj) & 0xff) << 24) | (((min) & 0xff) << 16) | ((mic) & 0xffff))

#if CS_WIN32


#define CS_FORCEINLINE __forceinline

#define CS_ZERO(e) memset(&e, 0, sizeof(e))

#else

#define CS_FORCEINLINE inline

#endif