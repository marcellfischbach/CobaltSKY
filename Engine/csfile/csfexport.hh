
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)





#ifdef CSF_EXPORT
#define CSF_API __declspec(dllexport)
#else
#define CSF_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CSF_API

#endif

