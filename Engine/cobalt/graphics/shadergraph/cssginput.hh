#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/shadergraph/cssgenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csdefs.hh>

#include <string>

class csSGNode;
class csSGOutput;


class CSE_API csSGInput
{
  friend class csSGNode;
public:
  csSGInput(const std::string &name, bool canConst, bool canInputNode);
  virtual ~csSGInput();

  void SetIdx(csUInt32 idx);
  csUInt32 GetIdx() const;

  const csSGNode *GetNode() const;
  csSGNode *GetNode();

  const std::string &GetName() const;

  void SetInput(csSGOutput* output);
  csSGOutput* GetInput();
  const csSGOutput* GetInput() const;

  bool CanInputNode() const;
  csSGNode *GetInputNode();

  bool CanInputConst() const;
  void SetConst(float fl);
  float GetConst() const;

  csSGDataType GetDataType()const;

private:
  std::string m_name;

  csSGNode *m_node;

  bool m_canInputNode;
  csSGOutput *m_input;

  bool m_canInputConst;
  float m_constFloat;

  csUInt32 m_idx;

};

CS_FORCEINLINE void csSGInput::SetIdx(csUInt32 idx)
{
  m_idx = idx;
}
CS_FORCEINLINE csUInt32 csSGInput::GetIdx() const
{
  return m_idx;
}


CS_FORCEINLINE const std::string &csSGInput::GetName() const
{
  return m_name;
}

CS_FORCEINLINE csSGNode *csSGInput::GetNode()
{
  return m_node;
}

CS_FORCEINLINE const csSGNode *csSGInput::GetNode() const
{
  return m_node;
}


CS_FORCEINLINE csSGOutput *csSGInput::GetInput()
{
  return m_input;
}

CS_FORCEINLINE const csSGOutput *csSGInput::GetInput() const
{
  return m_input;
}

CS_FORCEINLINE bool csSGInput::CanInputNode() const
{
  return m_canInputNode;
}

CS_FORCEINLINE bool csSGInput::CanInputConst() const
{
  return m_canInputConst;
}

CS_FORCEINLINE void csSGInput::SetConst(float fl)
{
  m_constFloat = fl;
}

CS_FORCEINLINE float csSGInput::GetConst() const
{
  return m_constFloat;
}
