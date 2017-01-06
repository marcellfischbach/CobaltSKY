#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/graphics/shadergraph/vksgenums.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/vkdefs.hh>


class vkSGNode;


class VKE_API vkSGOutput
{
  friend class vkSGNode;
public:
  vkSGOutput(vkSGDataType type, const vkString &name, const vkString &attr = "");
  vkSGOutput(const vkString &name, const vkString &attr = "");
  virtual ~vkSGOutput();

  void SetIdx(vkUInt32 idx);
  vkUInt32 GetIdx() const;

  const vkSGNode *GetNode() const;
  vkSGNode *GetNode();

  void SetDataType(vkSGDataType dataType);
  vkSGDataType GetDataType() const;
  const vkString &GetName() const;
  const vkString &GetAttr() const;

private:
  vkSGDataType m_dataType;
  vkString m_name;
  vkString m_attr;

  vkSGNode *m_node;

  vkUInt32 m_idx;

};


VK_FORCEINLINE void vkSGOutput::SetIdx(vkUInt32 idx)
{
  m_idx = idx;
}
VK_FORCEINLINE vkUInt32 vkSGOutput::GetIdx() const
{
  return m_idx;
}


VK_FORCEINLINE void vkSGOutput::SetDataType(vkSGDataType dataType)
{
  m_dataType = dataType;
}

VK_FORCEINLINE vkSGDataType vkSGOutput::GetDataType() const
{
  return m_dataType;
}

VK_FORCEINLINE const vkString &vkSGOutput::GetName() const
{
  return m_name;
}

VK_FORCEINLINE const vkString &vkSGOutput::GetAttr() const
{
  return m_attr;
}

VK_FORCEINLINE vkSGNode *vkSGOutput::GetNode()
{
  return m_node;
}

VK_FORCEINLINE const vkSGNode *vkSGOutput::GetNode() const
{
  return m_node;
}