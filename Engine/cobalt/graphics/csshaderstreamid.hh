#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <string>

namespace cs
{

class CSE_API ShaderStreamID
{
public:
  ShaderStreamID(const std::string& attribute);
  ShaderStreamID(csUInt32 id);

  const std::string& GetName() const;
  csUInt32 GetID() const;

  bool operator== (const cs::ShaderStreamID& other) const;
  bool operator< (const cs::ShaderStreamID& other) const;
private:
  std::string m_name;
  csUInt32 m_id;
};

}
