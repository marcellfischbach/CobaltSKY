#pragma once

namespace cs
{

enum eBufferAccessMode
{
  eBAM_Read,
  eBAM_Write,
  eBAM_ReadWrite,
};

enum eBufferDataMode
{
  eBDM_Dynamic,
  eBDM_Static,
  eBDM_Stream
};

enum eButton
{
  eB_Left,
  eB_Middle,
  eB_Right,
  eB_COUNT
};

enum eKey
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
  eK_Space,
  eK_Left,
  eK_Right,
  eK_Up,
  eK_Down,
  eK_COUNT,
};

enum ePrimitiveType
{
  ePT_Points,
  ePT_Lines,
  ePT_Triangles,
};

enum eVertexStreamType
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

  eVST_ParticleSize,
  eVST_ParticleRotation,
  eVST_ParticleTextPage,
  eVST_ParticleTimeToLive,

  eVST_BoneIndex,
  eVST_BoneWeight,

  eVST_COUNT
};

enum eVertexAttributeType
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
  eVAT_SkeletonMapping,


  eVAT_Texture0,
  eVAT_Texture1,
  eVAT_Texture2,
  eVAT_Texture3,
  eVAT_Texture4,
  eVAT_Texture5,
  eVAT_Texture6,
  eVAT_Texture7,


  eVAT_ViewportSize,
  eVAT_ViewportSizeInv,
  eVAT_BinaryGradient,
  eVAT_BinaryGradientToScreen,
  eVAT_FadeInOutValue,
  eVAT_FadeInOutDist,
  eVAT_Blend,

  eVAT_COUNT
};

enum eMatrixType
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

enum eDataType
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

enum eFilterMode
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

enum eTextureAddressMode
{
  eTAM_Repeat,
  eTAM_RepeatMirror,
  eTAM_Clamp,
  eTAM_ClampBorder,
  eTAM_MirrowOnce,
};

enum eTextureUnit
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

enum eTextureType
{
  eTT_Texture1D,
  eTT_Texture2D,
  eTT_Texture3D,
  eTT_TextureCube,
  eTT_Texture1DArray,
  eTT_Texture2DArray,
};

enum eTextureCubeFace
{
  eTCF_PositiveX,
  eTCF_NegativeX,
  eTCF_PositiveY,
  eTCF_NegativeY,
  eTCF_PositiveZ,
  eTCF_NegativeZ,
};

enum eShaderParameterType
{
  eSPT_Float,
  eSPT_Vector2,
  eSPT_Vector3,
  eSPT_Vector4,
  eSPT_Int,
  eSPT_IVector2,
  eSPT_IVector3,
  eSPT_IVector4,
  eSPT_Color4,
  eSPT_Matrix3,
  eSPT_Matrix4,
  eSPT_Texture,
};

enum ePixelFormat
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

enum eTextureCompareMode
{
  eTCM_CompareToR,
  eTCM_None,
};

enum eTextureCompareFunc
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

enum eRenderPass
{
  eRP_GBuffer,
  //eRP_ShadowGeneric,
  eRP_ShadowPSSM,
  eRP_ShadowCube,
  eRP_ForwardUnlit,
  eRP_Forward1Light,
  eRP_Forward2Light,
  eRP_Forward3Light,
  eRP_Forward4Light,
  eRP_COUNT,
};

enum eRenderQueue
{
  eRQ_Deferred,
  eRQ_Particles,
  eRQ_Forward,
  eRQ_ForwardTransparent,
  eRQ_Unshaded,
  eRQ_COUNT,
};

enum eShadingMode
{
  eSM_Unlit,
  eSM_Shaded,
};


enum eParticleShadingMode
{
  ePSM_Shaded,
  ePSM_Emitting,
};

enum eFillMode
{
  eFM_Fill,
  eFM_Wireframe,
  eFM_Points
};

enum eFaceWinding
{
  eFW_CW,
  eFW_CCW
};

enum eFaceSide
{
  eFS_Front,
  eFS_Back,
  eFS_FrontAndBack,
};


enum eResourceLoadingMode
{
  eRLM_Shared,
  eRLM_Instance,
  eRLM_Inline,
};


enum eLightType
{
  eLT_PointLight,
  eLT_DirectionalLight,
  eLT_Count,
};

enum eProjectionMode
{
  ePM_Perspective,
  ePM_Orthographic,
};

enum eBlendMode
{
  eBM_Zero,
  eBM_One,
  eBM_SrcColor,
  eBM_InvSrcColor,
  eBM_SrcAlpha,
  eBM_InvSrcAlpha,
  eBM_DstColor,
  eBM_InvDstColor,
  eBM_DstAlpha,
  eBM_InvDstAlpha,
};


enum ePhysGeometryType
{
  ePGT_Sphere,
  ePGT_CylinderX,
  ePGT_CylinderY,
  ePGT_CylinderZ,
  ePGT_CapsuleX,
  ePGT_CapsuleY,
  ePGT_CapsuleZ,
  ePGT_Box,
  ePGT_Plane,
  ePGT_HeightField,
  ePGT_TriMesh,
};

enum ePhysicsColliderType
{
  ePCT_Static,
  ePCT_Kinematic,
  ePCT_Dynamic,
  ePCT_Trigger,
};


enum ePhysicsJointType
{
  ePJT_Hinge,
};



enum eCompareMode
{
  eCM_LessOrEqual,
  eCM_GreaterOrEqual,
  eCM_Less,
  eCM_Greater,
  eCM_Equal,
  eCM_NotEqual,
  eCM_Never,
  eCM_Always,
};


enum eRenderDestination
{
  eRD_Color0,
  eRD_Color1,
  eRD_Color2,
  eRD_Color3,
  eRD_Color4,
  eRD_Color5,
  eRD_Color6,
  eRD_Color7,
  eRD_Depth,
  eRD_Stencil
};

enum eParticleSpawnMode
{
  ePSM_Point,
  ePSM_Box,
  ePSM_Sphere,
};

enum eParticleSizeMode
{
  ePSM_Constant,
  ePSM_Linear,
  ePSM_Triangle,
  ePSM_Saw,
  ePSM_Pulse,
};

enum eParticleRotationMode
{
  ePRM_Pos,
  ePRM_Neg,
  ePRM_Both,
};

}
