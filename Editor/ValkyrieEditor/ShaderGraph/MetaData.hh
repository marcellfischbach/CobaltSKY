#pragma once


#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Vector.hh>
#include <map>
#include <ShaderGraph/MetaData.refl.hh>

VK_CLASS()
class ShaderGraphMetaData : public vkObject
{
  VK_CLASS_GEN;
public:
  ShaderGraphMetaData();
  virtual ~ShaderGraphMetaData();

  void SetGraphPosition(const vkVector2f &graphPosition);
  void SetNodePosition(vkUInt32 idx, const vkVector2f &position);
  vkVector2f GetGraphPosition() const;
  vkVector2f GetNodePosition(vkUInt32 idx) const;

private:
  vkVector2f m_graphPosition;
  std::map<vkUInt32, vkVector2f> m_nodePositions;
};