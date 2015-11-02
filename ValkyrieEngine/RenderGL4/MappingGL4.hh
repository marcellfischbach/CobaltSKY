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


static GLenum textureTypeMap[] = {
  GL_TEXTURE_1D, // eTT_Texture1D,
  GL_TEXTURE_2D, // eTT_Texture2D,
  GL_TEXTURE_3D, // eTT_Texture3D,
  GL_TEXTURE_CUBE_MAP, // eTT_TextureCube,
  GL_TEXTURE_1D_ARRAY, // eTT_Texture1DArray,
  GL_TEXTURE_2D_ARRAY, // eTT_Texture2DArray,
};

// 0x1908


static GLenum internalFormatMap[] = {
  GL_RGBA32F,
  GL_RGBA32I,
  GL_RGBA32UI,
  GL_RGB32F,
  GL_RGB32I,
  GL_RGB32UI,
  GL_RG32F,
  GL_RG32I,
  GL_RG32UI,
  GL_R32F,
  GL_R32I,
  GL_R32UI,
  GL_RGBA16F,
  GL_RGBA16I,
  GL_RGBA16UI,
  GL_RGB16F,
  GL_RGB16I,
  GL_RGB16UI,
  GL_RG16F,
  GL_RG16I,
  GL_RG16UI,
  GL_R16F,
  GL_R16I,
  GL_R16UI,
  GL_RGBA,
  GL_RGBA,
  GL_RGB,
  GL_RGB,
  GL_RG,
  GL_RG,
  GL_R,
  GL_R,
  GL_RGBA,
  GL_RGB,
  GL_RG,
  GL_R,
  GL_DEPTH32F_STENCIL8,
  GL_RGB10_A2,
};

static GLenum externalFormatMap[] = {
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RG,
  GL_RG,
  GL_RG,
  GL_R,
  GL_R,
  GL_R,
  GL_RGBA,
  GL_RGBA,
  GL_RGBA,
  GL_RGB,
  GL_RGB,
  GL_RGB,
  GL_RG,
  GL_RG,
  GL_RG,
  GL_R,
  GL_R,
  GL_R,
  GL_RGBA,
  GL_RGBA,
  GL_RGB,
  GL_RGB,
  GL_RG,
  GL_RG,
  GL_R,
  GL_R,
  GL_RGBA,
  GL_RGB,
  GL_RG,
  GL_R,
  0,
  GL_RGBA,
};

static GLenum externalFormatTypeMap[] = {
  GL_FLOAT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_FLOAT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_FLOAT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_FLOAT,
  GL_INT,
  GL_UNSIGNED_INT,
  GL_HALF_FLOAT,
  GL_SHORT,
  GL_UNSIGNED_SHORT,
  GL_HALF_FLOAT,
  GL_SHORT,
  GL_UNSIGNED_SHORT,
  GL_HALF_FLOAT,
  GL_SHORT,
  GL_UNSIGNED_SHORT,
  GL_HALF_FLOAT,
  GL_SHORT,
  GL_UNSIGNED_SHORT,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  GL_UNSIGNED_BYTE,
  0,
  GL_UNSIGNED_BYTE,
};
