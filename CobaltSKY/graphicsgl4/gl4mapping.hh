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
  GL_DEPTH24_STENCIL8,
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
  GL_DEPTH_STENCIL,
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
  GL_UNSIGNED_INT_24_8,
  GL_UNSIGNED_BYTE,
};

static unsigned pixelFormatSizeMap[] = {
  16, //ePF_R32G32B32A32F,
  16, //ePF_R32G32B32A32S,
  16, //ePF_R32G32B32A32U,
  12, //ePF_R32G32B32F,
  12, //ePF_R32G32B32S,
  12, //ePF_R32G32B32U,
  8, //ePF_R32G32F,
  8, //ePF_R32G32S,
  8, //ePF_R32G32U,
  4, //ePF_R32F,
  4, //ePF_R32S,
  4, //ePF_R32U,
  8, //ePF_R16G16B16A16F,
  8, //ePF_R16G16B16A16S,
  8, //ePF_R16G16B16A16U,
  6, //ePF_R16G16B16F,
  6, //ePF_R16G16B16S,
  6, //ePF_R16G16B16U,
  4, //ePF_R16G16F,
  4, //ePF_R16G16S,
  4, //ePF_R16G16U,
  2, //ePF_R16F,
  2, //ePF_R16S,
  2, //ePF_R16U,
  4, //ePF_R8G8B8A8S,
  4, //ePF_R8G8B8A8U,
  3, //ePF_R8G8B8S,
  3, //ePF_R8G8B8U,
  2, //ePF_R8G8S,
  2, //ePF_R8G8U,
  1, //ePF_R8S,
  1, //ePF_R8U,
  4, //ePF_RGBA,
  3, //ePF_RGB,
  2, //ePF_RG,
  1, //ePF_R,
  4, //ePF_D24S8,
  4, //ePF_R10G10B10A2,

};

static GLenum textureCompareModeMap[] = {
  GL_COMPARE_REF_TO_TEXTURE,
  GL_NONE,
};

static GLenum textureCompareFuncMap[] = {
  GL_LEQUAL,
  GL_GEQUAL,
  GL_LESS,
  GL_GREATER,
  GL_EQUAL,
  GL_NOTEQUAL,
  GL_ALWAYS,
  GL_NEVER,
};

static GLenum compareFuncMap[] = {
  GL_LEQUAL,
  GL_GEQUAL,
  GL_LESS,
  GL_GREATER,
  GL_EQUAL,
  GL_NOTEQUAL,
  GL_ALWAYS,
  GL_NEVER,
};



static GLenum blendModeMap[] = {
  GL_ZERO, // eBM_Zero,
  GL_ONE,  // eBM_One,
  GL_SRC_COLOR, // eBM_SrcColor,
  GL_ONE_MINUS_SRC_COLOR, // eBM_InvSrcColor,
  GL_SRC_ALPHA, // eBM_SrcAlpha,
  GL_ONE_MINUS_SRC_ALPHA, // eBM_InvSrcAlpha,
  GL_DST_COLOR, // eBM_DstColor,
  GL_ONE_MINUS_DST_COLOR, // eBM_InvDstColor,
  GL_DST_ALPHA, // eBM_DstAlpha,
  GL_ONE_MINUS_DST_ALPHA, // eBM_InvDstAlpha,
};


static GLenum renderDestinationMap[] = {
  GL_COLOR_ATTACHMENT0,
  GL_COLOR_ATTACHMENT1,
  GL_COLOR_ATTACHMENT2,
  GL_COLOR_ATTACHMENT3,
  GL_COLOR_ATTACHMENT4,
  GL_COLOR_ATTACHMENT5,
  GL_COLOR_ATTACHMENT6,
  GL_COLOR_ATTACHMENT7,
  GL_DEPTH_ATTACHMENT,
  GL_STENCIL_ATTACHMENT
};
