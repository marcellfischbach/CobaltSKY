
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef SAMPLEREDITOR_EXPORT
#define SAMPLEREDITOR_API __declspec(dllexport)
#else
#define SAMPLEREDITOR_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SAMPLEREDITOR_API

#endif

