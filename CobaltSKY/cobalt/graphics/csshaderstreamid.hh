#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csstring.hh>
#include <cobalt/cstypes.hh>



class CSE_API csShaderStreamID
{
public:
  csShaderStreamID(const csString &attribute);
  csShaderStreamID(csUInt32 id);

  const csString &GetName() const;
  csUInt32 GetID() const;

  bool operator== (const csShaderStreamID &other) const;
  bool operator< (const csShaderStreamID &other) const;
private:
  csString m_name;
  csUInt32 m_id;
};
