#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <string>



class CSE_API csShaderStreamID
{
public:
  csShaderStreamID(const std::string &attribute);
  csShaderStreamID(csUInt32 id);

  const std::string &GetName() const;
  csUInt32 GetID() const;

  bool operator== (const csShaderStreamID &other) const;
  bool operator< (const csShaderStreamID &other) const;
private:
  std::string m_name;
  csUInt32 m_id;
};
