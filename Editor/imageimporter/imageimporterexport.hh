
#pragma once

#ifdef CS_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef IMAGEIMPORTER_EXPORT
#define IMAGEIMPORTER_API __declspec(dllexport)
#else
#define IMAGEIMPORTER_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define IMAGEIMPORTER_API

#endif

