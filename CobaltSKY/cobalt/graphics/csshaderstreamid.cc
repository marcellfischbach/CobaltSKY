
#include <cobalt/graphics/csshaderstreamid.hh>
#include <cobalt/csenums.hh>
#include <map>


class csShaderStreamIDManager
{
public:
  static csShaderStreamIDManager *Get()
  {
    static csShaderStreamIDManager static_manager;
    return &static_manager;
  }

  csUInt32 Get(const csString &name)
  {
    std::map<csString, csUInt32>::iterator it = m_map.find(name);
    if (it != m_map.end())
    {
      return it->second;
    }

    csUInt32 id = m_nextID++;
    m_map[name] = id;
    return id;
  }

  const csString &Get(csUInt32 id)
  {
    std::map<csString, csUInt32>::iterator it;
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
  csShaderStreamIDManager()
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


  std::map<csString, csUInt32> m_map;
  csUInt32 m_nextID;
  csString m_nullString;
};


csShaderStreamID::csShaderStreamID(const csString &name)
  : m_name(name)
  , m_id(csShaderStreamIDManager::Get()->Get(name))
{
}

csShaderStreamID::csShaderStreamID(csUInt32 id)
  : m_name(csShaderStreamIDManager::Get()->Get(id))
  , m_id(id)
{

}

const csString &csShaderStreamID::GetName() const
{
  return m_name;
}

csUInt32 csShaderStreamID::GetID() const
{
  return m_id;
}


bool csShaderStreamID::operator==(const csShaderStreamID &other) const
{
  return m_id == other.m_id;
}


bool csShaderStreamID::operator<(const csShaderStreamID &other) const
{
  return m_id < other.m_id;
}

