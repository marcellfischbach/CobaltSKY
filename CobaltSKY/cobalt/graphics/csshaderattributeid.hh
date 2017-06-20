#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/core/csstring.hh>
#include <cobalt/cstypes.hh>



class CSE_API csShaderAttributeID
{
public:
  csShaderAttributeID(const csString &attribute);
  explicit csShaderAttributeID(csUInt32 id);

  const csString &GetName() const;
  csUInt32 GetID() const;

  bool operator== (const csShaderAttributeID &other) const;
  bool operator< (const csShaderAttributeID &other) const;
private:
  csString m_name;
  csUInt32 m_id;
};
