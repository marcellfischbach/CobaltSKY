#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/shadergraph/cssgenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csdefs.hh>
#include <string>

namespace cs
{

class SGNode;


class CSE_API SGOutput
{
  friend class cs::SGNode;
public:
  SGOutput(cs::eSGDataType type, const std::string& name, const std::string& attr = "");
  SGOutput(const std::string& name, const std::string& attr = "");
  virtual ~SGOutput();

  void SetIdx(csUInt32 idx);
  csUInt32 GetIdx() const;

  const cs::SGNode* GetNode() const;
  cs::SGNode* GetNode();

  void SetDataType(cs::eSGDataType dataType);
  cs::eSGDataType GetDataType() const;
  const std::string& GetName() const;
  const std::string& GetAttr() const;

private:
  cs::eSGDataType m_dataType;
  std::string m_name;
  std::string m_attr;

  cs::SGNode* m_node;

  csUInt32 m_idx;

};

}


CS_FORCEINLINE void cs::SGOutput::SetIdx(csUInt32 idx)
{
  m_idx = idx;
}
CS_FORCEINLINE csUInt32 cs::SGOutput::GetIdx() const
{
  return m_idx;
}


CS_FORCEINLINE void cs::SGOutput::SetDataType(cs::eSGDataType dataType)
{
  m_dataType = dataType;
}

CS_FORCEINLINE cs::eSGDataType cs::SGOutput::GetDataType() const
{
  return m_dataType;
}

CS_FORCEINLINE const std::string &cs::SGOutput::GetName() const
{
  return m_name;
}

CS_FORCEINLINE const std::string &cs::SGOutput::GetAttr() const
{
  return m_attr;
}

CS_FORCEINLINE cs::SGNode *cs::SGOutput::GetNode()
{
  return m_node;
}

CS_FORCEINLINE const cs::SGNode *cs::SGOutput::GetNode() const
{
  return m_node;
}