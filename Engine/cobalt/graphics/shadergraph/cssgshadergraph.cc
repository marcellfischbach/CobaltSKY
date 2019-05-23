

#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <map>


cs::SGShaderGraph::SGShaderGraph()
  : cs::MaterialDef()
  , m_diffuse(0)
  , m_roughness(0)
  , m_alpha(0)
  , m_discardAlpha(false)
  , m_discardAlphaThreshold(0.0f)
  , m_discardAlphaCompareMode(cs::eCM_Less)
  , m_normal(0)
  , m_skinnedMaterial(false)
  , m_maxBones(64)
{

}

void cs::SGShaderGraph::Clear()
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

void cs::SGShaderGraph::SetInput(cs::SGShaderGraph::InputType type, cs::SGOutput *output)
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

cs::SGOutput *cs::SGShaderGraph::GetInput(cs::SGShaderGraph::InputType type)
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

const cs::SGOutput *cs::SGShaderGraph::GetInput(cs::SGShaderGraph::InputType type) const
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

bool cs::SGShaderGraph::Validate()
{
  bool success = true;

  success &= ValidateNode(m_diffuse);
  success &= ValidateNode(m_alpha);
  success &= ValidateNode(m_roughness);
  success &= ValidateNode(m_normal);

  return success;
}


cs::SGNode *cs::SGShaderGraph::GetNode(cs::SGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return output->GetNode();
}

bool cs::SGShaderGraph::ValidateNode(cs::SGOutput *output)
{
  cs::SGNode *node = GetNode(output);
  if (!node)
  {
    return true;
  }

  return node->Validate();
}


void cs::SGShaderGraph::AddNode(cs::SGNode *node)
{
  if (!node)
  {
    return;
  }
  node->AddRef();
  m_allNodes.push_back(node);
}

size_t cs::SGShaderGraph::GetNumberOfTotalNodes() const
{
  return m_allNodes.size();
}

cs::SGNode* cs::SGShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

const cs::SGNode* cs::SGShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

bool cs::SGShaderGraph::ContainsNode(const cs::SGNode *node) const
{
  for (cs::SGNode *n : m_allNodes)
  {
    if (n == node)
    {
      return true;
    }
  }

  return false;
}


int cs::SGShaderGraph::GetIndexOfNode(const cs::SGNode *node) const
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

void cs::SGShaderGraph::RemoveNode(cs::SGNode *node)
{
  int i = GetIndexOfNode(node);
  RemoveNode((size_t)i);
}

void cs::SGShaderGraph::RemoveNode(size_t idx)
{
  std::vector<cs::SGNode*>::iterator it = m_allNodes.begin() + idx;
  if (it != m_allNodes.end())
  {
    cs::SGNode *node = *it;
    if (node)
    {
      node->Release();
    }
  }
  m_allNodes.erase(it);
}

cs::SGShaderGraph *cs::SGShaderGraph::Copy(cs::SGShaderGraph *dest) const
{
  if (!dest)
  {
    dest = new cs::SGShaderGraph();
  }

  dest->Clear();

  //
  // create a duplicate of all nodes
  std::map<const cs::SGNode*, cs::SGNode*> mapping;
  for (cs::SGNode *node : m_allNodes)
  {
    cs::SGNode *newNode = node->GetClass()->CreateInstance<cs::SGNode>();
    newNode = node->Copy(newNode);

    dest->m_allNodes.push_back(newNode);
    mapping[node] = newNode;
  }
  for (cs::SGNode *node : m_allNodes)
  {
    cs::SGNode *newNode = mapping[node];
    for (csSize i=0, in = node->GetNumberOfInputs(); i < in; ++i)
    {
      cs::SGInput *input = node->GetInput(i);
      cs::SGInput *newInput = newNode->GetInput(i);

      if (input->CanInputConst())
      {
        newInput->SetConst(input->GetConst());
      }

      cs::SGOutput *output = input->GetInput();
      if (output)
      {
        cs::SGNode *outputNode = output->GetNode();
        cs::SGNode *newOutputNode = mapping[outputNode];
        cs::SGOutput *newOutput = newOutputNode->GetOutput(output->GetIdx());
        newInput->SetInput(newOutput);
      }
    }
  }

  for (csSize i = 0; i < eIT_COUNT; ++i)
  {
    const cs::SGOutput *output = GetInput((InputType)i);
    if (output)
    {
      const cs::SGNode *outputNode = output->GetNode();
      cs::SGNode *newOutputNode = mapping[outputNode];
      cs::SGOutput *newOutput = newOutputNode->GetOutput(output->GetIdx());
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

