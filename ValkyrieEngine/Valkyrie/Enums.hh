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

enum vkPrimitiveType
{
  ePT_Points,
  ePT_Lines,
  ePT_Triangles,
};

enum vkVertexStreamType
{
  eVST_Position,
  eVST_Normal,
  eVST_Color,
  eVST_Tangent,
  eVST_BiNormal,

  eVST_Position1,
  eVST_Normal1,
  eVST_Color1,
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
  eVAT_MatProjView,
  eVAT_MatProjViewInv,
  eVAT_MatViewModel,
  eVAT_MatViewModelInv,
  eVAT_MatProjViewModel,
  eVAT_MatProjViewModelInv,

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
  eMT_MatProjView,
  eMT_MatProjViewInv,
  eMT_MatViewModel,
  eMT_MatViewModelInv,
  eMT_MatProjViewModel,
  eMT_MatProjViewModelInv,
  eMT_COUNT
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

enum vkFilterMode
{
  eFM_MinMagNearest,
  eFM_MinNearestMagLinear,
  eFM_MinLinearMagNearest,
  eFM_MinMagLinear,
  eFM_MinMagMipNearest,
  eFM_MinMagNearestMipLinear,
  eFM_MinNearestMagLinearMipNearest,
  eFM_MinNearestMagMipLinear,
  eFM_MinLinearMagMipNearest,
  eFM_MinLinearMagNearestMipLinear,
  eFM_MinMagLinearMipNearest,
  eFM_MinMagMipLinear,
  eFM_Anisotropic,
};

enum vkTextureAddressMode
{
  eTAM_Repeat,
  eTAM_RepeatMirror,
  eTAM_Clamp,
  eTAM_ClampBorder,
  eTAM_MirrowOnce,
};

enum vkTextureUnit
{
  eTU_TextureUnit0,
  eTU_TextureUnit1,
  eTU_TextureUnit2,
  eTU_TextureUnit3,
  eTU_TextureUnit4,
  eTU_TextureUnit5,
  eTU_TextureUnit6,
  eTU_TextureUnit7,
  eTU_TextureUnit8,
  eTU_TextureUnit9,
  eTU_COUNT,
  eTU_Invalid = eTU_COUNT,
};

enum vkTextureType
{
  eTT_Texture1D,
  eTT_Texture2D,
  eTT_Texture3D,
  eTT_TextureCube,
  eTT_Texture1DArray,
  eTT_Texture2DArray,
};

enum vkShaderParameterType
{
  eSPT_Float,
  eSPT_Vector2,
  eSPT_Vector3,
  eSPT_Vector4,
  eSPT_Int,
  eSPT_IVector2,
  eSPT_IVector3,
  eSPT_IVector4,
  eSPT_Matrix3,
  eSPT_Matrix4,
  eSPT_Texture,
};

enum vkPixelFormat
{
  ePF_R32G32B32A32F,
  ePF_R32G32B32A32S,
  ePF_R32G32B32A32U,
  ePF_R32G32B32F,
  ePF_R32G32B32S,
  ePF_R32G32B32U,
  ePF_R32G32F,
  ePF_R32G32S,
  ePF_R32G32U,
  ePF_R32F,
  ePF_R32S,
  ePF_R32U,
  ePF_R16G16B16A16F,
  ePF_R16G16B16A16S,
  ePF_R16G16B16A16U,
  ePF_R16G16B16F,
  ePF_R16G16B16S,
  ePF_R16G16B16U,
  ePF_R16G16F,
  ePF_R16G16S,
  ePF_R16G16U,
  ePF_R16F,
  ePF_R16S,
  ePF_R16U,
  ePF_R8G8B8A8S,
  ePF_R8G8B8A8U,
  ePF_R8G8B8S,
  ePF_R8G8B8U,
  ePF_R8G8S,
  ePF_R8G8U,
  ePF_R8S,
  ePF_R8U,
  ePF_RGBA,
  ePF_RGB,
  ePF_RG,
  ePF_R,
  ePF_D24S8,
  ePF_R10G10B10A2,
};

enum vkTextureCompareMode
{
  eTCM_CompareToR,
  eTCM_None,
};

enum vkTextureCompareFunc
{
  eTCF_LessOrEqual,
  eTCF_GreaterOrEqual,
  eTCF_Less,
  eTCF_Greater,
  eTCF_Equal,
  eTCF_NotEqual,
  eTCF_Always,
  eTCF_Never,

};

enum vkRenderPass
{
  eRP_GBuffer,
  eRP_COUNT,
};



enum vkResourceLoadingMode
{
  eRLM_Shared,
  eRLM_Instance,
  eRLM_Inline,
};


enum vkLightType
{
  eLT_PointLight,
  eLT_DirectionalLight,
  eLT_Count,
};

