

#include <valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <map>


vkSGShaderGraph::vkSGShaderGraph()
  : vkMaterial()
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

void vkSGShaderGraph::Clear()
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

void vkSGShaderGraph::SetInput(vkSGShaderGraph::InputType type, vkSGOutput *output)
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

vkSGOutput *vkSGShaderGraph::GetInput(vkSGShaderGraph::InputType type)
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

const vkSGOutput *vkSGShaderGraph::GetInput(vkSGShaderGraph::InputType type) const
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

bool vkSGShaderGraph::Validate()
{
  bool success = true;

  success &= ValidateNode(m_diffuse);
  success &= ValidateNode(m_alpha);
  success &= ValidateNode(m_roughness);
  success &= ValidateNode(m_normal);

  return success;
}


vkSGNode *vkSGShaderGraph::GetNode(vkSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return output->GetNode();
}

bool vkSGShaderGraph::ValidateNode(vkSGOutput *output)
{
  vkSGNode *node = GetNode(output);
  if (!node)
  {
    return true;
  }

  return node->Validate();
}


void vkSGShaderGraph::AddNode(vkSGNode *node)
{
  if (!node)
  {
    return;
  }
  node->AddRef();
  m_allNodes.push_back(node);
}

size_t vkSGShaderGraph::GetNumberOfTotalNodes() const
{
  return m_allNodes.size();
}

vkSGNode* vkSGShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

const vkSGNode* vkSGShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

bool vkSGShaderGraph::ContainsNode(const vkSGNode *node) const
{
  for (vkSGNode *n : m_allNodes)
  {
    if (n == node)
    {
      return true;
    }
  }

  return false;
}


int vkSGShaderGraph::GetIndexOfNode(const vkSGNode *node) const
{
  for (vkSize i=0, in=m_allNodes.size(); i<in; ++i)
  {
    if (m_allNodes[i] == node)
    {
      return (int)i;
    }
  }

  return -1;
}

void vkSGShaderGraph::RemoveNode(vkSGNode *node)
{
  int i = GetIndexOfNode(node);
  RemoveNode((size_t)i);
}

void vkSGShaderGraph::RemoveNode(size_t idx)
{
  std::vector<vkSGNode*>::iterator it = m_allNodes.begin() + idx;
  if (it != m_allNodes.end())
  {
    vkSGNode *node = *it;
    if (node)
    {
      node->Release();
    }
  }
  m_allNodes.erase(it);
}

vkSGShaderGraph *vkSGShaderGraph::Copy(vkSGShaderGraph *dest) const
{
  if (!dest)
  {
    dest = new vkSGShaderGraph();
  }

  dest->Clear();

  //
  // create a duplicate of all nodes
  std::map<const vkSGNode*, vkSGNode*> mapping;
  for (vkSGNode *node : m_allNodes)
  {
    vkSGNode *newNode = node->GetClass()->CreateInstance<vkSGNode>();
    newNode = node->Copy(newNode);

    dest->m_allNodes.push_back(newNode);
    mapping[node] = newNode;
  }
  for (vkSGNode *node : m_allNodes)
  {
    vkSGNode *newNode = mapping[node];
    for (vkSize i=0, in = node->GetNumberOfInputs(); i < in; ++i)
    {
      vkSGInput *input = node->GetInput(i);
      vkSGInput *newInput = newNode->GetInput(i);

      if (input->CanInputConst())
      {
        newInput->SetConst(input->GetConst());
      }

      vkSGOutput *output = input->GetInput();
      if (output)
      {
        vkSGNode *outputNode = output->GetNode();
        vkSGNode *newOutputNode = mapping[outputNode];
        vkSGOutput *newOutput = newOutputNode->GetOutput(output->GetIdx());
        newInput->SetInput(newOutput);
      }
    }
  }

  for (vkSize i = 0; i < eIT_COUNT; ++i)
  {
    const vkSGOutput *output = GetInput((InputType)i);
    if (output)
    {
      const vkSGNode *outputNode = output->GetNode();
      vkSGNode *newOutputNode = mapping[outputNode];
      vkSGOutput *newOutput = newOutputNode->GetOutput(output->GetIdx());
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