
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)



#ifdef SAMPLER_EDITOR_EXPORT
#define SAMPLER_EDITOR_API __declspec(dllexport)
#else
#define SAMPLER_EDITOR_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SAMPLER_EDITOR_API

#endif

