#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/csmaterialdef.hh>

#include <cobalt/graphics/shadergraph/cssgshadergraph.refl.hh>

namespace cs
{

class SGNode;
class SGOutput;

CS_CLASS()
class CSE_API SGShaderGraph : public CS_SUPER(cs::MaterialDef)
{
  CS_CLASS_GEN;
public:
  enum InputType
  {
    eIT_START,
    eIT_Diffuse = eIT_START,
    eIT_Alpha,
    eIT_Roughness,
    eIT_Normal,
    eIT_Undefined,
    eIT_COUNT = eIT_Undefined,
  };
public:
  SGShaderGraph();

  void Clear();

  bool Validate();

  void SetInput(InputType type, cs::SGOutput * output);
  cs::SGOutput* GetInput(InputType type);
  const cs::SGOutput* GetInput(InputType type) const;

  void SetDiffuse(cs::SGOutput * diffuse);
  cs::SGOutput* GetDiffuse();
  const cs::SGOutput* GetDiffuse() const;

  void SetRoughness(cs::SGOutput * roughness);
  cs::SGOutput* GetRoughness();
  const cs::SGOutput* GetRoughness() const;

  void SetAlpha(cs::SGOutput * alpha);
  cs::SGOutput* GetAlpha();
  const cs::SGOutput* GetAlpha() const;

  void SetBlendOutWithBinaryGradient(bool blendOutWithBinaryGradient);
  bool IsBlendOutWithBinaryGradient() const;

  void SetSkinnedMaterial(bool skinnedMaterial);
  bool IsSkinnedMaterial() const;

  void SetMaxBones(unsigned maxBones);
  unsigned GetMaxBones() const;

  void SetDiscardAlpha(bool discardAlpha);
  void SetDiscardAlpha(float discardAlphaThreshold, cs::eCompareMode discardAlphaCompareMode);
  bool IsDiscardAlpha() const;
  float GetDiscardAlphaThreshold() const;
  cs::eCompareMode GetDiscardAlphaCompareMode() const;

  void SetNormal(cs::SGOutput * normal);
  cs::SGOutput* GetNormal();
  const cs::SGOutput* GetNormal() const;

  void AddNode(cs::SGNode * node);
  size_t GetNumberOfTotalNodes() const;
  cs::SGNode* GetNode(size_t idx);
  const cs::SGNode* GetNode(size_t idx) const;
  bool ContainsNode(const cs::SGNode * node) const;
  int GetIndexOfNode(const cs::SGNode * node) const;
  void RemoveNode(cs::SGNode * node);
  void RemoveNode(size_t idx);

  cs::SGShaderGraph* Copy(cs::SGShaderGraph * dest) const;

private:
  cs::SGNode* GetNode(cs::SGOutput * output);
  bool ValidateNode(cs::SGOutput * output);
  bool m_blendOutWithBinaryGradient;
  bool m_skinnedMaterial;
  unsigned m_maxBones;

  cs::SGOutput* m_diffuse;
  cs::SGOutput* m_roughness;

  cs::SGOutput* m_alpha;
  bool m_discardAlpha;
  float m_discardAlphaThreshold;
  cs::eCompareMode m_discardAlphaCompareMode;

  cs::SGOutput* m_normal;

  std::vector<cs::SGNode*> m_allNodes;

};

CS_CLASS()
class CSE_API SGShaderGraphWrapper : public CS_SUPER(cs::MaterialDefWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::SGShaderGraph, SGShaderGraphWrapper, cs::MaterialDefWrapper);
};


struct CSE_API iSGShaderGraphLogger
{
  virtual ~iSGShaderGraphLogger() { }
  virtual void LogSourceCode(const std::string& renderPass, const std::string& shaderType, const std::string& sourceCode) = 0;
  virtual void LogInfo(const std::string& renderPass, const std::string& msg) = 0;
  virtual void LogError(const std::string& renderPass, const std::string& msg) = 0;
};

struct CSE_API iSGShaderGraphFactory
{
  virtual ~iSGShaderGraphFactory() { }

  virtual bool GenerateShaderGraph(cs::SGShaderGraph* graph, cs::iSGShaderGraphLogger* logger = 0) = 0;
};

}



CS_FORCEINLINE void cs::SGShaderGraph::SetDiffuse(cs::SGOutput *diffuse)
{
  m_diffuse = diffuse;
}

CS_FORCEINLINE cs::SGOutput *cs::SGShaderGraph::GetDiffuse()
{
  return m_diffuse;
}


CS_FORCEINLINE const cs::SGOutput *cs::SGShaderGraph::GetDiffuse() const
{
  return m_diffuse;
}


CS_FORCEINLINE void cs::SGShaderGraph::SetRoughness(cs::SGOutput *roughness)
{
  m_roughness = roughness;
}

CS_FORCEINLINE cs::SGOutput *cs::SGShaderGraph::GetRoughness()
{
  return m_roughness;
}


CS_FORCEINLINE const cs::SGOutput *cs::SGShaderGraph::GetRoughness() const
{
  return m_roughness;
}


CS_FORCEINLINE void cs::SGShaderGraph::SetAlpha(cs::SGOutput *alpha)
{
  m_alpha = alpha;
}

CS_FORCEINLINE cs::SGOutput *cs::SGShaderGraph::GetAlpha()
{
  return m_alpha;
}


CS_FORCEINLINE const cs::SGOutput *cs::SGShaderGraph::GetAlpha() const
{
  return m_alpha;
}

CS_FORCEINLINE void cs::SGShaderGraph::SetDiscardAlpha(bool discardAlpha)
{
  m_discardAlpha = discardAlpha;
}

CS_FORCEINLINE void cs::SGShaderGraph::SetDiscardAlpha(float discardAlphaThreshold, cs::eCompareMode discardAlphaCompareMode)
{
  m_discardAlphaThreshold = discardAlphaThreshold;
  m_discardAlphaCompareMode = discardAlphaCompareMode;
}

CS_FORCEINLINE bool cs::SGShaderGraph::IsDiscardAlpha() const
{
  return m_discardAlpha;
}

CS_FORCEINLINE float cs::SGShaderGraph::GetDiscardAlphaThreshold() const
{
  return m_discardAlphaThreshold;
}

CS_FORCEINLINE cs::eCompareMode cs::SGShaderGraph::GetDiscardAlphaCompareMode() const
{
  return m_discardAlphaCompareMode;
}

CS_FORCEINLINE void cs::SGShaderGraph::SetBlendOutWithBinaryGradient(bool blendOutWithBinaryGradient)
{
  m_blendOutWithBinaryGradient = blendOutWithBinaryGradient;
}

CS_FORCEINLINE bool cs::SGShaderGraph::IsBlendOutWithBinaryGradient() const
{
  return m_blendOutWithBinaryGradient;
}


CS_FORCEINLINE void cs::SGShaderGraph::SetSkinnedMaterial(bool skinnedMaterial)
{
  m_skinnedMaterial = skinnedMaterial;
}

CS_FORCEINLINE bool cs::SGShaderGraph::IsSkinnedMaterial() const
{
  return m_skinnedMaterial;
}


CS_FORCEINLINE void cs::SGShaderGraph::SetMaxBones(unsigned maxBones)
{
  m_maxBones = maxBones;
}

CS_FORCEINLINE unsigned cs::SGShaderGraph::GetMaxBones() const
{
  return m_maxBones;
}


CS_FORCEINLINE void cs::SGShaderGraph::SetNormal(cs::SGOutput *normal)
{
  m_normal = normal;
}

CS_FORCEINLINE cs::SGOutput *cs::SGShaderGraph::GetNormal()
{
  return m_normal;
}


CS_FORCEINLINE const cs::SGOutput *cs::SGShaderGraph::GetNormal() const
{
  return m_normal;
}
