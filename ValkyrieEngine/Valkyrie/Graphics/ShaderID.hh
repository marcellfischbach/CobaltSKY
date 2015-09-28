#pragma once



#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Types.hh>



class VKE_API vkShaderAttributeID
{
public:
  vkShaderAttributeID(const vkString &attribute);
  vkShaderAttributeID(vkUInt32 id);

  const vkString &ResolveName();
  const vkString &GetName() const;
  vkUInt32 GetID() const;
private:
  vkString m_name;
  vkUInt32 m_id;
};

class VKE_API vkShaderStreamID
{
public:
  vkShaderStreamID(const vkString &attribute);
  vkShaderStreamID(vkUInt32 id);

  const vkString &ResolveName();
  const vkString &GetName() const;
  vkUInt32 GetID() const;
private:
  vkString m_name;
  vkUInt32 m_id;
};
