#pragma once

enum vkSGDataType
{
  eSGDT_Inval = 0x00,
  eSGDT_Int = 0x01,
  eSGDT_Int2 = 0x02,
  eSGDT_Int3 = 0x04,
  eSGDT_Int4 = 0x08,
  eSGDT_Float = 0x10,
  eSGDT_Float2 = 0x20,
  eSGDT_Float3 = 0x40,
  eSGDT_Float4 = 0x80,
  eSGDT_IntTypes = 0x0f,
  eSGDT_FloatTypes = 0xf0,
  eSGDT_ScalarTypes = 0xff,

  eSGDT_Texture1D = 0x100,
  eSGDT_Texture1DArray = 0x200,
  eSGDT_Texture2D = 0x400,
  eSGDT_Texture2DArray = 0x800,
  eSGDT_Texture3D = 0x1000,
  eSGDT_TextureCube = 0x2000,
  eSGDT_TextureCubeArray = 0x4000,
  eSGDT_Textures = 0x7f00,
};
