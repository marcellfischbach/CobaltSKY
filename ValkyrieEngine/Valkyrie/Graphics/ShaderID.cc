
#include <Valkyrie/Graphics/ShaderID.hh>
#include <Valkyrie/Enums.hh>
#include <map>

class vkShaderAttributeIDManager
{
public:
  static vkShaderAttributeIDManager *Get()
  {
    static vkShaderAttributeIDManager static_manager;
    return &static_manager;
  }

  vkUInt32 Get(const vkString &name)
  {
    std::map<vkString, vkUInt32>::iterator it = m_map.find(name);
    if (it != m_map.end())
    {
      return it->second;
    }

    vkUInt32 id = m_nextID++;
    m_map[name] = id;
    return id;
  }

  const vkString &Get(vkUInt32 id)
  {
    std::map<vkString, vkUInt32>::iterator it;
    for (it = m_map.begin(); it != m_map.end(); ++it)
    {
      if (it->second == id)
      {
        return it->first;
      }
    }
    return m_nullString;
  }

private:
  vkShaderAttributeIDManager()
  {
    m_nullString = "";

    m_map["MatProj"] = eVAT_MatProj;
    m_map["MatProjInv"] = eVAT_MatProjInv;
    m_map["MatView"] = eVAT_MatView;
    m_map["MatViewInv"] = eVAT_MatViewInv;
    m_map["MatModel"] = eVAT_MatModel;
    m_map["MatModelInv"] = eVAT_MatModelInv;
    m_map["MatViewProj"] = eVAT_MatViewProj;
    m_map["MatViewProjInv"] = eVAT_MatViewProjInv;
    m_map["MatModelView"] = eVAT_MatModelView;
    m_map["MatModelViewInv"] = eVAT_MatModelViewInv;
    m_map["MatModelViewProj"] = eVAT_MatModelViewProj;
    m_map["MatModelViewProjInv"] = eVAT_MatModelViewProjInv;

    m_map["MatsSkeleton"] = eVAT_MatsSkeleton;


    m_map["Texture0"] = eVAT_Texture0;
    m_map["Texture1"] = eVAT_Texture1;
    m_map["Texture2"] = eVAT_Texture2;
    m_map["Texture3"] = eVAT_Texture3;
    m_map["Texture4"] = eVAT_Texture4;
    m_map["Texture5"] = eVAT_Texture5;
    m_map["Texture6"] = eVAT_Texture6;
    m_map["Texture7"] = eVAT_Texture7;


    m_map["Blend"] = eVAT_Blend;
    m_nextID = eVAT_COUNT;

  }


  std::map<vkString, vkUInt32> m_map;
  vkUInt32 m_nextID;
  vkString m_nullString;
};



class vkShaderStreamIDManager
{
public:
  static vkShaderStreamIDManager *Get()
  {
    static vkShaderStreamIDManager static_manager;
    return &static_manager;
  }

  vkUInt32 Get(const vkString &name)
  {
    std::map<vkString, vkUInt32>::iterator it = m_map.find(name);
    if (it != m_map.end())
    {
      return it->second;
    }

    vkUInt32 id = m_nextID++;
    m_map[name] = id;
    return id;
  }

  const vkString &Get(vkUInt32 id)
  {
    std::map<vkString, vkUInt32>::iterator it;
    for (it = m_map.begin(); it != m_map.end(); ++it)
    {
      if (it->second == id)
      {
        return it->first;
      }
    }
    return m_nullString;
  }

private:
  vkShaderStreamIDManager()
  {
    m_nullString = "";
    m_map["Position"] = eVST_Position;
    m_map["Normal"] = eVST_Normal;
    m_map["Color"] = eVST_Color;
    m_map["Tangent"] = eVST_Tangent;
    m_map["BiNormal"] = eVST_BiNormal;

    m_map["Position1"] = eVST_Position1;
    m_map["Normal1"] = eVST_Normal1;
    m_map["Color1"] = eVST_Color1;
    m_map["Tangent1"] = eVST_Tangent1;
    m_map["BiNormal1"] = eVST_BiNormal1;

    m_map["TexCoord0"] = eVST_TexCoord0;
    m_map["TexCoord1"] = eVST_TexCoord1;
    m_map["TexCoord2"] = eVST_TexCoord2;
    m_map["TexCoord3"] = eVST_TexCoord3;
    m_map["TexCoord4"] = eVST_TexCoord4;
    m_map["TexCoord5"] = eVST_TexCoord5;
    m_map["TexCoord6"] = eVST_TexCoord6;
    m_map["TexCoord7"] = eVST_TexCoord7;

    m_map["BoneIndex"] = eVST_BoneIndex;
    m_map["BoneWeight"] = eVST_BoneWeight;

    m_nextID = eVST_COUNT;

  }


  std::map<vkString, vkUInt32> m_map;
  vkUInt32 m_nextID;
  vkString m_nullString;
};

vkShaderAttributeID::vkShaderAttributeID(const vkString &name)
  : m_name(name)
  , m_id(vkShaderAttributeIDManager::Get()->Get(name))
{
}

vkShaderAttributeID::vkShaderAttributeID(vkUInt32 id)
  : m_name("")
  , m_id(id)
{

}

const vkString &vkShaderAttributeID::GetName() const
{
  return m_name;
}

vkUInt32 vkShaderAttributeID::GetID() const
{
  return m_id;
}


const vkString &vkShaderAttributeID::ResolveName()
{
  if (m_name.length() == 0)
  {
    m_name = vkShaderAttributeIDManager::Get()->Get(m_id);
  }
  return m_name;
}



vkShaderStreamID::vkShaderStreamID(const vkString &name)
  : m_name(name)
  , m_id(vkShaderStreamIDManager::Get()->Get(name))
{
}

vkShaderStreamID::vkShaderStreamID(vkUInt32 id)
  : m_name("")
  , m_id(id)
{

}

const vkString &vkShaderStreamID::GetName() const
{
  return m_name;
}

vkUInt32 vkShaderStreamID::GetID() const
{
  return m_id;
}


const vkString &vkShaderStreamID::ResolveName()
{
  if (m_name.length() == 0)
  {
    m_name = vkShaderStreamIDManager::Get()->Get(m_id);
  }
  return m_name;
}


