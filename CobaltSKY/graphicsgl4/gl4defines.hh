#pragma once


#include <assert.h>
#include <stdio.h>

#define CS_GL_OFFSET(offset)  ((const GLvoid*)((unsigned long long)offset))

#if 1

 #define CS_DEFINE_GL_ERROR_CHECK 

#if defined(_DEBUG) && defined(CS_DEFINE_GL_ERROR_CHECK)
#	define CS_CHECK_GL_ERROR \
	{\
	  GLenum error = glGetError ();\
	  if (error != GL_NO_ERROR)\
		{\
		  printf ("GL_ERROR: 0x%08x\n", error);\
		  printf ("Error   : %s\n", (const char*)gluErrorString(error));\
		  fflush (stdout);\
		}\
		\
	  assert(error == GL_NO_ERROR);\
	}
#else
#	define CS_CHECK_GL_ERROR
#endif
#else
# define CS_CHECK_GL_ERROR
#endif

#if 0
#ifdef PROFILE_GL_FLUSH
#	define CS_GL_FLUSH glFlush()
#else
#	define CS_GL_FLUSH
#endif
#else
# define CS_GL_FLUSH
#endif
