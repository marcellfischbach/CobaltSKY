

#include <ShaderGraph/MetaData.hh>

ShaderGraphMetaData::ShaderGraphMetaData()
  : vkObject()
{

}

ShaderGraphMetaData::~ShaderGraphMetaData()
{

}

void ShaderGraphMetaData::SetGraphPosition(const vkVector2f &graphPosition)
{
  m_graphPosition = graphPosition;
}

void ShaderGraphMetaData::SetNodePosition(vkUInt32 idx, const vkVector2f &nodePosition)
{
  m_nodePositions[idx] = nodePosition;
}

vkVector2f ShaderGraphMetaData::GetGraphPosition() const
{
  return m_graphPosition;
}

vkVector2f ShaderGraphMetaData::GetNodePosition(vkUInt32 idx) const
{
  auto it = m_nodePositions.find(idx);
  if (it == m_nodePositions.end())
  {
    return vkVector2f(0.0f, 0.0f);
  }
  
  return it->second;
}
