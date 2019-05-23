
#include <cobalt/graphics/csshaderattributeid.hh>
#include <cobalt/csenums.hh>
#include <map>

namespace cs
{

class ShaderAttributeIDManager
{
public:
  static ShaderAttributeIDManager* Get()
  {
    static ShaderAttributeIDManager static_manager;
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
  ShaderAttributeIDManager()
  {
    m_nullString = "";

    m_map["MatProj"] = cs::eVAT_MatProj;
    m_map["MatProjInv"] = cs::eVAT_MatProjInv;
    m_map["MatView"] = cs::eVAT_MatView;
    m_map["MatViewInv"] = cs::eVAT_MatViewInv;
    m_map["MatModel"] = cs::eVAT_MatModel;
    m_map["MatModelInv"] = cs::eVAT_MatModelInv;
    m_map["MatProjView"] = cs::eVAT_MatProjView;
    m_map["MatProjViewInv"] = cs::eVAT_MatProjViewInv;
    m_map["MatViewModel"] = cs::eVAT_MatViewModel;
    m_map["MatViewModelInv"] = cs::eVAT_MatViewModelInv;
    m_map["MatProjViewModel"] = cs::eVAT_MatProjViewModel;
    m_map["MatProjViewModelInv"] = cs::eVAT_MatProjViewModelInv;

    m_map["MatsSkeleton"] = cs::eVAT_MatsSkeleton;


    m_map["Texture0"] = cs::eVAT_Texture0;
    m_map["Texture1"] = cs::eVAT_Texture1;
    m_map["Texture2"] = cs::eVAT_Texture2;
    m_map["Texture3"] = cs::eVAT_Texture3;
    m_map["Texture4"] = cs::eVAT_Texture4;
    m_map["Texture5"] = cs::eVAT_Texture5;
    m_map["Texture6"] = cs::eVAT_Texture6;
    m_map["Texture7"] = cs::eVAT_Texture7;

    m_map["ViewportSize"] = cs::eVAT_ViewportSize;
    m_map["ViewportSizeInv"] = cs::eVAT_ViewportSizeInv;

    m_map["BinaryGradient"] = cs::eVAT_BinaryGradient;
    m_map["BinaryGradientToScreen"] = cs::eVAT_BinaryGradientToScreen;
    m_map["FadeInOutDist"] = cs::eVAT_FadeInOutDist;
    m_map["FadeInOutValue"] = cs::eVAT_FadeInOutValue;

    m_map["Blend"] = cs::eVAT_Blend;
    m_nextID = cs::eVAT_COUNT;

  }


  std::map<std::string, csUInt32> m_map;
  csUInt32 m_nextID;
  std::string m_nullString;
};

}


cs::ShaderAttributeID::ShaderAttributeID(const std::string &name)
  : m_name(name)
  , m_id(cs::ShaderAttributeIDManager::Get()->Get(name))
{
}

cs::ShaderAttributeID::ShaderAttributeID(csUInt32 id)
  : m_name(cs::ShaderAttributeIDManager::Get()->Get(id))
  , m_id(id)
{

}

const std::string &cs::ShaderAttributeID::GetName() const
{
  return m_name;
}

csUInt32 cs::ShaderAttributeID::GetID() const
{
  return m_id;
}



bool cs::ShaderAttributeID::operator==(const cs::ShaderAttributeID &other) const
{
  return m_id == other.m_id;
}
bool cs::ShaderAttributeID::operator<(const cs::ShaderAttributeID &other) const
{
  return m_id < other.m_id;
}

