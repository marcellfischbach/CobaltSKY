#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <string>



class CSE_API csShaderAttributeID
{
public:
  csShaderAttributeID(const std::string &attribute);
  explicit csShaderAttributeID(csUInt32 id);

  const std::string &GetName() const;
  csUInt32 GetID() const;

  bool operator== (const csShaderAttributeID &other) const;
  bool operator< (const csShaderAttributeID &other) const;
private:
  std::string m_name;
  csUInt32 m_id;
};
