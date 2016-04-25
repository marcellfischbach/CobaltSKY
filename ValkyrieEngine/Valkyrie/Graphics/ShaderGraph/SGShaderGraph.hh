#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.refl.hh>

class vkSGNode;
class vkSGOutput;

VK_CLASS()
class VKE_API vkSGShaderGraph : public vkMaterial
{
  VK_CLASS_GEN;
public:
  enum InputType
  {
    eIT_Diffuse,
    eIT_Alpha,
    eIT_Roughness,
    eIT_Normal,
    eIT_Undefined,
  };
public:
  vkSGShaderGraph();

  bool Validate();

  void SetInput(InputType type, vkSGOutput* output);
  vkSGOutput *GetInput(InputType type);
  const vkSGOutput *GetInput(InputType type) const;

  void SetDiffuse(vkSGOutput *diffuse);
  vkSGOutput *GetDiffuse();
  const vkSGOutput *GetDiffuse() const;

  void SetRoughness(vkSGOutput *roughness);
  vkSGOutput *GetRoughness();
  const vkSGOutput *GetRoughness() const;

  void SetAlpha(vkSGOutput *alpha);
  vkSGOutput *GetAlpha();
  const vkSGOutput *GetAlpha() const;

  void SetBlendOutWithBinaryGradient(bool blendOutWithBinaryGradient);
  bool IsBlendOutWithBinaryGradient() const;

  void SetDiscardAlpha(bool discardAlpha);
  void SetDiscardAlpha(float discardAlphaThreshold, vkCompareMode discardAlphaCompareMode);
  bool IsDiscardAlpha() const;
  float GetDiscardAlphaThreshold() const;
  vkCompareMode GetDiscardAlphaCompareMode() const;

  void SetNormal(vkSGOutput *normal);
  vkSGOutput *GetNormal();
  const vkSGOutput *GetNormal() const;

private:
  vkSGNode *GetNode(vkSGOutput *output);
  bool ValidateNode(vkSGOutput *output);
  bool m_blendOutWithBinaryGradient;
  vkSGOutput *m_diffuse;
  vkSGOutput *m_roughness;

  vkSGOutput *m_alpha;
  bool m_discardAlpha;
  float m_discardAlphaThreshold;
  vkCompareMode m_discardAlphaCompareMode;

  vkSGOutput *m_normal;

};

struct VKE_API ISGShaderGraphFactory 
{
  virtual ~ISGShaderGraphFactory() { }

  virtual bool GenerateShaderGraph(vkSGShaderGraph *graph) = 0;
};


VK_FORCEINLINE void vkSGShaderGraph::SetDiffuse(vkSGOutput *diffuse)
{
  m_diffuse = diffuse;
}

VK_FORCEINLINE vkSGOutput *vkSGShaderGraph::GetDiffuse()
{
  return m_diffuse;
}


VK_FORCEINLINE const vkSGOutput *vkSGShaderGraph::GetDiffuse() const
{
  return m_diffuse;
}


VK_FORCEINLINE void vkSGShaderGraph::SetRoughness(vkSGOutput *roughness)
{
  m_roughness = roughness;
}

VK_FORCEINLINE vkSGOutput *vkSGShaderGraph::GetRoughness()
{
  return m_roughness;
}


VK_FORCEINLINE const vkSGOutput *vkSGShaderGraph::GetRoughness() const
{
  return m_roughness;
}


VK_FORCEINLINE void vkSGShaderGraph::SetAlpha(vkSGOutput *alpha)
{
  m_alpha = alpha;
}

VK_FORCEINLINE vkSGOutput *vkSGShaderGraph::GetAlpha()
{
  return m_alpha;
}


VK_FORCEINLINE const vkSGOutput *vkSGShaderGraph::GetAlpha() const
{
  return m_alpha;
}

VK_FORCEINLINE void vkSGShaderGraph::SetDiscardAlpha(bool discardAlpha)
{
  m_discardAlpha = discardAlpha;
}

VK_FORCEINLINE void vkSGShaderGraph::SetDiscardAlpha(float discardAlphaThreshold, vkCompareMode discardAlphaCompareMode)
{
  m_discardAlphaThreshold = discardAlphaThreshold;
  m_discardAlphaCompareMode = discardAlphaCompareMode;
}

VK_FORCEINLINE bool vkSGShaderGraph::IsDiscardAlpha() const
{
  return m_discardAlpha;
}

VK_FORCEINLINE float vkSGShaderGraph::GetDiscardAlphaThreshold() const
{
  return m_discardAlphaThreshold;
}

VK_FORCEINLINE vkCompareMode vkSGShaderGraph::GetDiscardAlphaCompareMode() const
{
  return m_discardAlphaCompareMode;
}

VK_FORCEINLINE void vkSGShaderGraph::SetBlendOutWithBinaryGradient(bool blendOutWithBinaryGradient)
{
  m_blendOutWithBinaryGradient = blendOutWithBinaryGradient;
}

VK_FORCEINLINE bool vkSGShaderGraph::IsBlendOutWithBinaryGradient() const
{
  return m_blendOutWithBinaryGradient;
}

VK_FORCEINLINE void vkSGShaderGraph::SetNormal(vkSGOutput *normal)
{
  m_normal = normal;
}

VK_FORCEINLINE vkSGOutput *vkSGShaderGraph::GetNormal()
{
  return m_normal;
}


VK_FORCEINLINE const vkSGOutput *vkSGShaderGraph::GetNormal() const
{
  return m_normal;
}