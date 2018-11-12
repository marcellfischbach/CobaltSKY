

#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <map>


csSGShaderGraph::csSGShaderGraph()
  : csMaterialDef()
  , m_diffuse(0)
  , m_roughness(0)
  , m_alpha(0)
  , m_discardAlpha(false)
  , m_discardAlphaThreshold(0.0f)
  , m_discardAlphaCompareMode(eCM_Less)
  , m_normal(0)
  , m_skinnedMaterial(false)
  , m_maxBones(64)
{

}

void csSGShaderGraph::Clear()
{
  for (size_t i = eIT_START; i < eIT_COUNT; ++i)
  {
    SetInput((InputType)i, 0);
  }

  for (size_t i = 0, in = m_allNodes.size(); i < in; ++i)
  {
    m_allNodes[i]->Release();
  }
  m_allNodes.clear();
}

void csSGShaderGraph::SetInput(csSGShaderGraph::InputType type, csSGOutput *output)
{
  switch (type)
  {
  case eIT_Diffuse:
    SetDiffuse(output);
    break;
  case eIT_Roughness:
    SetRoughness(output);
    break;
  case eIT_Alpha:
    SetAlpha(output);
    break;
  case eIT_Normal:
    SetNormal(output);
    break;
  }
}

csSGOutput *csSGShaderGraph::GetInput(csSGShaderGraph::InputType type)
{
  switch (type)
  {
  case eIT_Diffuse:
    return GetDiffuse();
  case eIT_Roughness:
    return GetRoughness();
  case eIT_Alpha:
    return GetAlpha();
  case eIT_Normal:
    return GetNormal();
  }
  return 0;
}

const csSGOutput *csSGShaderGraph::GetInput(csSGShaderGraph::InputType type) const
{
  switch (type)
  {
  case eIT_Diffuse:
    return GetDiffuse();
  case eIT_Roughness:
    return GetRoughness();
  case eIT_Alpha:
    return GetAlpha();
  case eIT_Normal:
    return GetNormal();
  }
  return 0;
}

bool csSGShaderGraph::Validate()
{
  bool success = true;

  success &= ValidateNode(m_diffuse);
  success &= ValidateNode(m_alpha);
  success &= ValidateNode(m_roughness);
  success &= ValidateNode(m_normal);

  return success;
}


csSGNode *csSGShaderGraph::GetNode(csSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return output->GetNode();
}

bool csSGShaderGraph::ValidateNode(csSGOutput *output)
{
  csSGNode *node = GetNode(output);
  if (!node)
  {
    return true;
  }

  return node->Validate();
}


void csSGShaderGraph::AddNode(csSGNode *node)
{
  if (!node)
  {
    return;
  }
  node->AddRef();
  m_allNodes.push_back(node);
}

size_t csSGShaderGraph::GetNumberOfTotalNodes() const
{
  return m_allNodes.size();
}

csSGNode* csSGShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

const csSGNode* csSGShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

bool csSGShaderGraph::ContainsNode(const csSGNode *node) const
{
  for (csSGNode *n : m_allNodes)
  {
    if (n == node)
    {
      return true;
    }
  }

  return false;
}


int csSGShaderGraph::GetIndexOfNode(const csSGNode *node) const
{
  for (csSize i=0, in=m_allNodes.size(); i<in; ++i)
  {
    if (m_allNodes[i] == node)
    {
      return (int)i;
    }
  }

  return -1;
}

void csSGShaderGraph::RemoveNode(csSGNode *node)
{
  int i = GetIndexOfNode(node);
  RemoveNode((size_t)i);
}

void csSGShaderGraph::RemoveNode(size_t idx)
{
  std::vector<csSGNode*>::iterator it = m_allNodes.begin() + idx;
  if (it != m_allNodes.end())
  {
    csSGNode *node = *it;
    if (node)
    {
      node->Release();
    }
  }
  m_allNodes.erase(it);
}

csSGShaderGraph *csSGShaderGraph::Copy(csSGShaderGraph *dest) const
{
  if (!dest)
  {
    dest = new csSGShaderGraph();
  }

  dest->Clear();

  //
  // create a duplicate of all nodes
  std::map<const csSGNode*, csSGNode*> mapping;
  for (csSGNode *node : m_allNodes)
  {
    csSGNode *newNode = node->GetClass()->CreateInstance<csSGNode>();
    newNode = node->Copy(newNode);

    dest->m_allNodes.push_back(newNode);
    mapping[node] = newNode;
  }
  for (csSGNode *node : m_allNodes)
  {
    csSGNode *newNode = mapping[node];
    for (csSize i=0, in = node->GetNumberOfInputs(); i < in; ++i)
    {
      csSGInput *input = node->GetInput(i);
      csSGInput *newInput = newNode->GetInput(i);

      if (input->CanInputConst())
      {
        newInput->SetConst(input->GetConst());
      }

      csSGOutput *output = input->GetInput();
      if (output)
      {
        csSGNode *outputNode = output->GetNode();
        csSGNode *newOutputNode = mapping[outputNode];
        csSGOutput *newOutput = newOutputNode->GetOutput(output->GetIdx());
        newInput->SetInput(newOutput);
      }
    }
  }

  for (csSize i = 0; i < eIT_COUNT; ++i)
  {
    const csSGOutput *output = GetInput((InputType)i);
    if (output)
    {
      const csSGNode *outputNode = output->GetNode();
      csSGNode *newOutputNode = mapping[outputNode];
      csSGOutput *newOutput = newOutputNode->GetOutput(output->GetIdx());
      dest->SetInput((InputType)i, newOutput);
    }
  }


  //
  // mirror the scalar attributes of the shader graph
  dest->m_blendOutWithBinaryGradient = m_blendOutWithBinaryGradient;
  dest->m_skinnedMaterial = m_skinnedMaterial;
  dest->m_maxBones = m_maxBones;
  dest->m_discardAlpha = m_discardAlpha;
  dest->m_discardAlphaCompareMode = m_discardAlphaCompareMode;
  dest->m_discardAlphaThreshold = m_discardAlphaThreshold;

  return dest;
}

