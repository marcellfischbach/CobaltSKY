#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csstring.hh>
#include <cobalt/graphics/shadergraph/cssgenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csdefs.hh>


class csSGNode;


class CSE_API csSGOutput
{
  friend class csSGNode;
public:
  csSGOutput(csSGDataType type, const csString &name, const csString &attr = "");
  csSGOutput(const csString &name, const csString &attr = "");
  virtual ~csSGOutput();

  void SetIdx(csUInt32 idx);
  csUInt32 GetIdx() const;

  const csSGNode *GetNode() const;
  csSGNode *GetNode();

  void SetDataType(csSGDataType dataType);
  csSGDataType GetDataType() const;
  const csString &GetName() const;
  const csString &GetAttr() const;

private:
  csSGDataType m_dataType;
  csString m_name;
  csString m_attr;

  csSGNode *m_node;

  csUInt32 m_idx;

};


CS_FORCEINLINE void csSGOutput::SetIdx(csUInt32 idx)
{
  m_idx = idx;
}
CS_FORCEINLINE csUInt32 csSGOutput::GetIdx() const
{
  return m_idx;
}


CS_FORCEINLINE void csSGOutput::SetDataType(csSGDataType dataType)
{
  m_dataType = dataType;
}

CS_FORCEINLINE csSGDataType csSGOutput::GetDataType() const
{
  return m_dataType;
}

CS_FORCEINLINE const csString &csSGOutput::GetName() const
{
  return m_name;
}

CS_FORCEINLINE const csString &csSGOutput::GetAttr() const
{
  return m_attr;
}

CS_FORCEINLINE csSGNode *csSGOutput::GetNode()
{
  return m_node;
}

CS_FORCEINLINE const csSGNode *csSGOutput::GetNode() const
{
  return m_node;
}