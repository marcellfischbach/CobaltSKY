#pragma once

#include <GL/glew.h>

static GLenum bufferDataModeMap[] = {
  GL_DYNAMIC_DRAW,	// BDM_Dynamic
  GL_STATIC_DRAW,		// BDM_Static
  GL_STREAM_DRAW		// BDM_Stream
};


static GLenum bufferAccessModeMap[] = {
  GL_READ_ONLY,
  GL_WRITE_ONLY,
  GL_READ_WRITE
};
