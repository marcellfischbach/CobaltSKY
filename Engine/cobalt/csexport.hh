
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef CSE_EXPORT
#define CSE_API __declspec(dllexport)
#else
#define CSE_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CSE_API

#endif

