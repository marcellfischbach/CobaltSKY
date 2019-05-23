#pragma once

namespace cs
{

enum ePostProcessOutput
{
  ePPO_GBuffer_Depth,
  ePPO_GBuffer_DiffuseRoughness,
  ePPO_GBuffer_NormalLightMode,
  ePPO_GBuffer_EmissiveMetallic,
  ePPO_GBuffer_SSSSpec,
  ePPO_FinalTarget_Color,
  ePPO_FinalTarget_Depth,
  ePPO_COUNT
};

}