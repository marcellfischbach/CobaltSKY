#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/graphics/shadergraph/vksgenums.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/vkdefs.hh>


class vkSGNode;
class vkSGOutput;


class VKE_API vkSGInput
{
  friend class vkSGNode;
public:
  vkSGInput(const vkString &name, bool canConst, bool canInputNode);
  virtual ~vkSGInput();

  void SetIdx(vkUInt32 idx);
  vkUInt32 GetIdx() const;

  const vkSGNode *GetNode() const;
  vkSGNode *GetNode();

  const vkString &GetName() const;

  void SetInput(vkSGOutput* output);
  vkSGOutput* GetInput();
  const vkSGOutput* GetInput() const;

  bool CanInputNode() const;
  vkSGNode *GetInputNode();

  bool CanInputConst() const;
  void SetConst(float fl);
  float GetConst() const;

  vkSGDataType GetDataType()const;

private:
  vkString m_name;

  vkSGNode *m_node;

  bool m_canInputNode;
  vkSGOutput *m_input;

  bool m_canInputConst;
  float m_constFloat;

  vkUInt32 m_idx;

};

VK_FORCEINLINE void vkSGInput::SetIdx(vkUInt32 idx)
{
  m_idx = idx;
}
VK_FORCEINLINE vkUInt32 vkSGInput::GetIdx() const
{
  return m_idx;
}


VK_FORCEINLINE const vkString &vkSGInput::GetName() const
{
  return m_name;
}

VK_FORCEINLINE vkSGNode *vkSGInput::GetNode()
{
  return m_node;
}

VK_FORCEINLINE const vkSGNode *vkSGInput::GetNode() const
{
  return m_node;
}


VK_FORCEINLINE vkSGOutput *vkSGInput::GetInput()
{
  return m_input;
}

VK_FORCEINLINE const vkSGOutput *vkSGInput::GetInput() const
{
  return m_input;
}

VK_FORCEINLINE bool vkSGInput::CanInputNode() const
{
  return m_canInputNode;
}

VK_FORCEINLINE bool vkSGInput::CanInputConst() const
{
  return m_canInputConst;
}

VK_FORCEINLINE void vkSGInput::SetConst(float fl)
{
  m_constFloat = fl;
}

VK_FORCEINLINE float vkSGInput::GetConst() const
{
  return m_constFloat;
}
