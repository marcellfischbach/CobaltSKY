#pragma once

#include <string>

#define null ((void*)0)

#ifdef WIN32

typedef unsigned __int8  vkUInt8;
typedef unsigned __int16 vkUInt16;
typedef unsigned __int32 vkUInt32;
typedef unsigned __int64 vkUInt64;

typedef signed __int8  vkInt8;
typedef signed __int16 vkInt16;
typedef signed __int32 vkInt32;
typedef signed __int64 vkInt64;

#endif


typedef std::string vkString;

typedef size_t vkSize;

