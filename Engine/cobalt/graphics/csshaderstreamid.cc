
#include <cobalt/graphics/csshaderstreamid.hh>
#include <cobalt/csenums.hh>
#include <map>

namespace cs
{

class ShaderStreamIDManager
{
public:
  static ShaderStreamIDManager* Get()
  {
    static ShaderStreamIDManager static_manager;
    return &static_manager;
  }

  csUInt32 Get(const std::string& name)
  {
    std::map<std::string, csUInt32>::iterator it = m_map.find(name);
    if (it != m_map.end())
    {
      return it->second;
    }

    csUInt32 id = m_nextID++;
    m_map[name] = id;
    return id;
  }

  const std::string& Get(csUInt32 id)
  {
    std::map<std::string, csUInt32>::iterator it;
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
  ShaderStreamIDManager()
  {
    m_nullString = "";
    m_map["Position"] = cs::eVST_Position;
    m_map["Normal"] = cs::eVST_Normal;
    m_map["Color"] = cs::eVST_Color;
    m_map["Tangent"] = cs::eVST_Tangent;
    m_map["BiNormal"] = cs::eVST_BiNormal;

    m_map["Position1"] = cs::eVST_Position1;
    m_map["Normal1"] = cs::eVST_Normal1;
    m_map["Color1"] = cs::eVST_Color1;
    m_map["Tangent1"] = cs::eVST_Tangent1;
    m_map["BiNormal1"] = cs::eVST_BiNormal1;

    m_map["TexCoord0"] = cs::eVST_TexCoord0;
    m_map["TexCoord1"] = cs::eVST_TexCoord1;
    m_map["TexCoord2"] = cs::eVST_TexCoord2;
    m_map["TexCoord3"] = cs::eVST_TexCoord3;
    m_map["TexCoord4"] = cs::eVST_TexCoord4;
    m_map["TexCoord5"] = cs::eVST_TexCoord5;
    m_map["TexCoord6"] = cs::eVST_TexCoord6;
    m_map["TexCoord7"] = cs::eVST_TexCoord7;

    m_map["ParticleSize"] = cs::eVST_ParticleSize;
    m_map["ParticleRotation"] = cs::eVST_ParticleRotation;
    m_map["ParticleTextPage"] = cs::eVST_ParticleTextPage;
    m_map["ParticleTimeToLive"] = cs::eVST_ParticleTimeToLive;

    m_map["BoneIndex"] = cs::eVST_BoneIndex;
    m_map["BoneWeight"] = cs::eVST_BoneWeight;

    m_nextID = cs::eVST_COUNT;

  }


  std::map<std::string, csUInt32> m_map;
  csUInt32 m_nextID;
  std::string m_nullString;
};

}


cs::ShaderStreamID::ShaderStreamID(const std::string &name)
  : m_name(name)
  , m_id(cs::ShaderStreamIDManager::Get()->Get(name))
{
}

cs::ShaderStreamID::ShaderStreamID(csUInt32 id)
  : m_name(cs::ShaderStreamIDManager::Get()->Get(id))
  , m_id(id)
{

}

const std::string &cs::ShaderStreamID::GetName() const
{
  return m_name;
}

csUInt32 cs::ShaderStreamID::GetID() const
{
  return m_id;
}


bool cs::ShaderStreamID::operator==(const cs::ShaderStreamID &other) const
{
  return m_id == other.m_id;
}


bool cs::ShaderStreamID::operator<(const cs::ShaderStreamID &other) const
{
  return m_id < other.m_id;
}

