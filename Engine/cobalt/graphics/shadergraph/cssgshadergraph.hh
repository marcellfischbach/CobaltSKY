#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmaterialdefwrapper.hh>

#include <cobalt/graphics/shadergraph/cssgshadergraph.refl.hh>

class csSGNode;
class csSGOutput;

CS_CLASS()
class CSE_API csSGShaderGraph : public CS_SUPER(csMaterialDef)
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
  csSGShaderGraph();

  void Clear();

  bool Validate();

  void SetInput(InputType type, csSGOutput* output);
  csSGOutput *GetInput(InputType type);
  const csSGOutput *GetInput(InputType type) const;

  void SetDiffuse(csSGOutput *diffuse);
  csSGOutput *GetDiffuse();
  const csSGOutput *GetDiffuse() const;

  void SetRoughness(csSGOutput *roughness);
  csSGOutput *GetRoughness();
  const csSGOutput *GetRoughness() const;

  void SetAlpha(csSGOutput *alpha);
  csSGOutput *GetAlpha();
  const csSGOutput *GetAlpha() const;

  void SetBlendOutWithBinaryGradient(bool blendOutWithBinaryGradient);
  bool IsBlendOutWithBinaryGradient() const;

  void SetSkinnedMaterial(bool skinnedMaterial);
  bool IsSkinnedMaterial() const;

  void SetMaxBones(unsigned maxBones);
  unsigned GetMaxBones() const;

  void SetDiscardAlpha(bool discardAlpha);
  void SetDiscardAlpha(float discardAlphaThreshold, csCompareMode discardAlphaCompareMode);
  bool IsDiscardAlpha() const;
  float GetDiscardAlphaThreshold() const;
  csCompareMode GetDiscardAlphaCompareMode() const;

  void SetNormal(csSGOutput *normal);
  csSGOutput *GetNormal();
  const csSGOutput *GetNormal() const;

  void AddNode(csSGNode *node);
  size_t GetNumberOfTotalNodes() const;
  csSGNode *GetNode(size_t idx) ;
  const csSGNode *GetNode(size_t idx) const;
  bool ContainsNode(const csSGNode *node) const;
  int GetIndexOfNode(const csSGNode *node) const;
  void RemoveNode(csSGNode *node);
  void RemoveNode(size_t idx);

  csSGShaderGraph *Copy(csSGShaderGraph *dest) const;

private:
  csSGNode *GetNode(csSGOutput *output);
  bool ValidateNode(csSGOutput *output);
  bool m_blendOutWithBinaryGradient;
  bool m_skinnedMaterial;
  unsigned m_maxBones;

  csSGOutput *m_diffuse;
  csSGOutput *m_roughness;

  csSGOutput *m_alpha;
  bool m_discardAlpha;
  float m_discardAlphaThreshold;
  csCompareMode m_discardAlphaCompareMode;

  csSGOutput *m_normal;

  std::vector<csSGNode*> m_allNodes;

};

class CSE_API csSGShaderGraphWrapper : public csMaterialDefWrapper
{
public:
  csSGShaderGraphWrapper(csSGShaderGraph *shaderGraph);
  virtual ~csSGShaderGraphWrapper();
};

struct CSE_API iSGShaderGraphLogger
{
  virtual void LogSourceCode(const std::string &renderPass, const std::string &shaderType, const std::string &sourceCode) = 0;
  virtual void LogInfo(const std::string &renderPass, const std::string &msg) = 0;
  virtual void LogError(const std::string &renderPass, const std::string &msg) = 0;
};

struct CSE_API ISGShaderGraphFactory 
{
  virtual ~ISGShaderGraphFactory() { }

  virtual bool GenerateShaderGraph(csSGShaderGraph *graph, iSGShaderGraphLogger *logger = 0) = 0;
};


CS_FORCEINLINE void csSGShaderGraph::SetDiffuse(csSGOutput *diffuse)
{
  m_diffuse = diffuse;
}

CS_FORCEINLINE csSGOutput *csSGShaderGraph::GetDiffuse()
{
  return m_diffuse;
}


CS_FORCEINLINE const csSGOutput *csSGShaderGraph::GetDiffuse() const
{
  return m_diffuse;
}


CS_FORCEINLINE void csSGShaderGraph::SetRoughness(csSGOutput *roughness)
{
  m_roughness = roughness;
}

CS_FORCEINLINE csSGOutput *csSGShaderGraph::GetRoughness()
{
  return m_roughness;
}


CS_FORCEINLINE const csSGOutput *csSGShaderGraph::GetRoughness() const
{
  return m_roughness;
}


CS_FORCEINLINE void csSGShaderGraph::SetAlpha(csSGOutput *alpha)
{
  m_alpha = alpha;
}

CS_FORCEINLINE csSGOutput *csSGShaderGraph::GetAlpha()
{
  return m_alpha;
}


CS_FORCEINLINE const csSGOutput *csSGShaderGraph::GetAlpha() const
{
  return m_alpha;
}

CS_FORCEINLINE void csSGShaderGraph::SetDiscardAlpha(bool discardAlpha)
{
  m_discardAlpha = discardAlpha;
}

CS_FORCEINLINE void csSGShaderGraph::SetDiscardAlpha(float discardAlphaThreshold, csCompareMode discardAlphaCompareMode)
{
  m_discardAlphaThreshold = discardAlphaThreshold;
  m_discardAlphaCompareMode = discardAlphaCompareMode;
}

CS_FORCEINLINE bool csSGShaderGraph::IsDiscardAlpha() const
{
  return m_discardAlpha;
}

CS_FORCEINLINE float csSGShaderGraph::GetDiscardAlphaThreshold() const
{
  return m_discardAlphaThreshold;
}

CS_FORCEINLINE csCompareMode csSGShaderGraph::GetDiscardAlphaCompareMode() const
{
  return m_discardAlphaCompareMode;
}

CS_FORCEINLINE void csSGShaderGraph::SetBlendOutWithBinaryGradient(bool blendOutWithBinaryGradient)
{
  m_blendOutWithBinaryGradient = blendOutWithBinaryGradient;
}

CS_FORCEINLINE bool csSGShaderGraph::IsBlendOutWithBinaryGradient() const
{
  return m_blendOutWithBinaryGradient;
}


CS_FORCEINLINE void csSGShaderGraph::SetSkinnedMaterial(bool skinnedMaterial)
{
  m_skinnedMaterial = skinnedMaterial;
}

CS_FORCEINLINE bool csSGShaderGraph::IsSkinnedMaterial() const
{
  return m_skinnedMaterial;
}


CS_FORCEINLINE void csSGShaderGraph::SetMaxBones(unsigned maxBones)
{
  m_maxBones = maxBones;
}

CS_FORCEINLINE unsigned csSGShaderGraph::GetMaxBones() const
{
  return m_maxBones;
}


CS_FORCEINLINE void csSGShaderGraph::SetNormal(csSGOutput *normal)
{
  m_normal = normal;
}

CS_FORCEINLINE csSGOutput *csSGShaderGraph::GetNormal()
{
  return m_normal;
}


CS_FORCEINLINE const csSGOutput *csSGShaderGraph::GetNormal() const
{
  return m_normal;
}
