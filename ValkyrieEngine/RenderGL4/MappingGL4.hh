#pragma once

#include <GL/glew.h>


static GLenum dataTypeMap[] = {
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_SHORT,
  GL_UNSIGNED_SHORT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_FLOAT,
  GL_DOUBLE
};

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


static GLenum shaderTypeMap[] = {
  GL_VERTEX_SHADER,
  GL_TESS_CONTROL_SHADER,
  GL_TESS_EVALUATION_SHADER,
  GL_GEOMETRY_SHADER,
  GL_FRAGMENT_SHADER,
  GL_COMPUTE_SHADER
};


static GLenum primitiveTypeMap[] = {
  GL_POINTS,
  GL_LINES,
  GL_TRIANGLES
};

static GLenum textureAddressModeMap[] = {
  GL_REPEAT,                  // eTAM_Repeat,
  GL_MIRRORED_REPEAT,         // eTAM_RepeatMirror,
  GL_CLAMP_TO_EDGE,           // eTAM_Clamp,
  GL_CLAMP_TO_BORDER,         // eTAM_ClampBorder,
  GL_MIRROR_CLAMP_TO_EDGE_EXT //eTAM_MirrowOnce,
};