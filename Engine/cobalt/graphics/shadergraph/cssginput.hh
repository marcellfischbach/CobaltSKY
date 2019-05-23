#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/shadergraph/cssgenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csdefs.hh>

#include <string>


namespace cs
{
class SGNode;
class SGOutput;

class CSE_API SGInput
{
  friend class cs::SGNode;
public:
  SGInput(const std::string& name, bool canConst, bool canInputNode);
  virtual ~SGInput();

  void SetIdx(csUInt32 idx);
  csUInt32 GetIdx() const;

  const cs::SGNode* GetNode() const;
  cs::SGNode* GetNode();

  const std::string& GetName() const;

  void SetInput(cs::SGOutput* output);
  cs::SGOutput* GetInput();
  const cs::SGOutput* GetInput() const;

  bool CanInputNode() const;
  cs::SGNode* GetInputNode();

  bool CanInputConst() const;
  void SetConst(float fl);
  float GetConst() const;

  cs::eSGDataType GetDataType()const;

private:
  std::string m_name;

  cs::SGNode* m_node;

  bool m_canInputNode;
  cs::SGOutput* m_input;

  bool m_canInputConst;
  float m_constFloat;

  csUInt32 m_idx;

};

}


CS_FORCEINLINE void cs::SGInput::SetIdx(csUInt32 idx)
{
  m_idx = idx;
}
CS_FORCEINLINE csUInt32 cs::SGInput::GetIdx() const
{
  return m_idx;
}


CS_FORCEINLINE const std::string &cs::SGInput::GetName() const
{
  return m_name;
}

CS_FORCEINLINE cs::SGNode *cs::SGInput::GetNode()
{
  return m_node;
}

CS_FORCEINLINE const cs::SGNode *cs::SGInput::GetNode() const
{
  return m_node;
}


CS_FORCEINLINE cs::SGOutput *cs::SGInput::GetInput()
{
  return m_input;
}

CS_FORCEINLINE const cs::SGOutput *cs::SGInput::GetInput() const
{
  return m_input;
}

CS_FORCEINLINE bool cs::SGInput::CanInputNode() const
{
  return m_canInputNode;
}

CS_FORCEINLINE bool cs::SGInput::CanInputConst() const
{
  return m_canInputConst;
}

CS_FORCEINLINE void cs::SGInput::SetConst(float fl)
{
  m_constFloat = fl;
}

CS_FORCEINLINE float cs::SGInput::GetConst() const
{
  return m_constFloat;
}

