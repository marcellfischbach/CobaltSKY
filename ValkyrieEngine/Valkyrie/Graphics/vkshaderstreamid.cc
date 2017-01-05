
#include <Valkyrie/Graphics/vkshaderstreamid.hh>
#include <Valkyrie/vkenums.hh>
#include <map>


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

    m_map["ParticleSize"] = eVST_ParticleSize;
    m_map["ParticleRotation"] = eVST_ParticleRotation;
    m_map["ParticleTextPage"] = eVST_ParticleTextPage;
    m_map["ParticleTimeToLive"] = eVST_ParticleTimeToLive;

    m_map["BoneIndex"] = eVST_BoneIndex;
    m_map["BoneWeight"] = eVST_BoneWeight;

    m_nextID = eVST_COUNT;

  }


  std::map<vkString, vkUInt32> m_map;
  vkUInt32 m_nextID;
  vkString m_nullString;
};


vkShaderStreamID::vkShaderStreamID(const vkString &name)
  : m_name(name)
  , m_id(vkShaderStreamIDManager::Get()->Get(name))
{
}

vkShaderStreamID::vkShaderStreamID(vkUInt32 id)
  : m_name(vkShaderStreamIDManager::Get()->Get(id))
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


bool vkShaderStreamID::operator==(const vkShaderStreamID &other) const
{
  return m_id == other.m_id;
}


bool vkShaderStreamID::operator<(const vkShaderStreamID &other) const
{
  return m_id < other.m_id;
}

