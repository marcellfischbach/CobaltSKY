#pragma once

enum vkBufferAccessMode
{
  eBAM_Read,
  eBAM_Write,
  eBAM_ReadWrite,
};

enum vkBufferDataMode
{
  eBDM_Dynamic,
  eBDM_Static,
  eBDM_Stream
};


enum vkKey
{
  eK_Undef,
  eK_A,
  eK_B,
  eK_C,
  eK_D,
  eK_E,
  eK_F,
  eK_G,
  eK_H,
  eK_I,
  eK_J,
  eK_K,
  eK_L,
  eK_M,
  eK_N,
  eK_O,
  eK_P,
  eK_Q,
  eK_R,
  eK_S,
  eK_T,
  eK_U,
  eK_V,
  eK_W,
  eK_X,
  eK_Y,
  eK_Z,
  eK_0,
  eK_1,
  eK_2,
  eK_3,
  eK_4,
  eK_5,
  eK_6,
  eK_7,
  eK_8,
  eK_9,
  eK_Esc,
  eK_LShift,
  eK_RShift,
  eK_LCtrl,
  eK_RCtrl,
  eK_COUNT,
};

enum vkVertexStreamType
{
  eVST_Position,
  eVST_Normal,
  eVST_Tangent,
  eVST_BiNormal,

  eVST_Position1,
  eVST_Normal1,
  eVST_Tangent1,
  eVST_BiNormal1,

  eVST_TexCoord0,
  eVST_TexCoord1,
  eVST_TexCoord2,
  eVST_TexCoord3,
  eVST_TexCoord4,
  eVST_TexCoord5,
  eVST_TexCoord6,
  eVST_TexCoord7,

  eVST_BoneIndex,
  eVST_BoneWeight,

  eVST_COUNT
};

enum vkVertexAttributeType
{
  eVAT_MatProj,
  eVAT_MatProjInv,
  eVAT_MatView,
  eVAT_MatViewInv,
  eVAT_MatModel,
  eVAT_MatModelInv,
  eVAT_MatViewProj,
  eVAT_MatViewProjInv,
  eVAT_MatModelView,
  eVAT_MatModelViewInv,
  eVAT_MatModelViewProj,
  eVAT_MatModelViewProjInv,

  eVAT_MatsSkeleton,


  eVAT_Texture0,
  eVAT_Texture1,
  eVAT_Texture2,
  eVAT_Texture3,
  eVAT_Texture4,
  eVAT_Texture5,
  eVAT_Texture6,
  eVAT_Texture7,

  eVAT_Blend,

  eVAT_COUNT
};

enum vkMatrixType
{
  eMT_MatProj,
  eMT_MatProjInv,
  eMT_MatView,
  eMT_MatViewInv,
  eMT_MatModel,
  eMT_MatModelInv,
  eMT_MatViewProj,
  eMT_MatViewProjInv,
  eMT_MatModelView,
  eMT_MatModelViewInv,
  eMT_MatModelViewProj,
  eMT_MatModelViewProjInv,
};

enum vkDataType
{
  eDT_Byte,
  eDT_UnsignedByte,
  eDT_Short,
  eDT_UnsignedShort,
  eDT_Int,
  eDT_UnsignedInt,
  eDT_Float,
  eDT_Double
};
