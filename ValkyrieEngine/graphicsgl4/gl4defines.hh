#pragma once


#include <assert.h>
#include <stdio.h>

#define VK_GL_OFFSET(offset)  ((const GLvoid*)((unsigned long)offset))

#if 1
#ifdef _DEBUG 
#	define VK_CHECK_GL_ERROR \
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
#	define VK_CHECK_GL_ERROR
#endif
#else
# define VK_CHECK_GL_ERROR
#endif

#if 0
#ifdef PROFILE_GL_FLUSH
#	define VK_GL_FLUSH glFlush()
#else
#	define VK_GL_FLUSH
#endif
#else
# define VK_GL_FLUSH
#endif
