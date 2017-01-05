#pragma once



#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkstring.hh>
#include <Valkyrie/vktypes.hh>



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
