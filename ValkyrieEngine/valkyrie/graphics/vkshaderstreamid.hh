#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/vktypes.hh>



class VKE_API vkShaderStreamID
{
public:
  vkShaderStreamID(const vkString &attribute);
  vkShaderStreamID(vkUInt32 id);

  const vkString &GetName() const;
  vkUInt32 GetID() const;

  bool operator== (const vkShaderStreamID &other) const;
  bool operator< (const vkShaderStreamID &other) const;
private:
  vkString m_name;
  vkUInt32 m_id;
};
