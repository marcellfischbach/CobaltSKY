#pragma once

#include <string>


#ifdef CS_WIN32

typedef unsigned __int8  csUInt8;
typedef unsigned __int16 csUInt16;
typedef unsigned __int32 csUInt32;
typedef unsigned __int64 csUInt64;

typedef signed __int8  csInt8;
typedef signed __int16 csInt16;
typedef signed __int32 csInt32;
typedef signed __int64 csInt64;

#endif


typedef unsigned long csID;
#define csID_Undefined ~0x00

typedef std::string csString;

typedef size_t csSize;

