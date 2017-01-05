#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkstring.hh>
#include <Valkyrie/vktypes.hh>



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
