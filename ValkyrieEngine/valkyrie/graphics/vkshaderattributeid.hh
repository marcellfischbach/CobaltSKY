#pragma once



#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/vktypes.hh>



class VKE_API vkShaderAttributeID
{
public:
  vkShaderAttributeID(const vkString &attribute);
  explicit vkShaderAttributeID(vkUInt32 id);

  const vkString &GetName() const;
  vkUInt32 GetID() const;

  bool operator== (const vkShaderAttributeID &other) const;
  bool operator< (const vkShaderAttributeID &other) const;
private:
  vkString m_name;
  vkUInt32 m_id;
};
