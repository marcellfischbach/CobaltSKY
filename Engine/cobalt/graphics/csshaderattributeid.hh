#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <string>


namespace cs
{


class CSE_API ShaderAttributeID
{
public:
  ShaderAttributeID(const std::string& attribute);
  explicit ShaderAttributeID(csUInt32 id);

  const std::string& GetName() const;
  csUInt32 GetID() const;

  bool operator== (const cs::ShaderAttributeID& other) const;
  bool operator< (const cs::ShaderAttributeID& other) const;
private:
  std::string m_name;
  csUInt32 m_id;
};

}

