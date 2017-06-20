
#include <cobalt/graphics/csshaderattributeid.hh>
#include <cobalt/csenums.hh>
#include <map>

class csShaderAttributeIDManager
{
public:
  static csShaderAttributeIDManager *Get()
  {
    static csShaderAttributeIDManager static_manager;
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
  csShaderAttributeIDManager()
  {
    m_nullString = "";

    m_map["MatProj"] = eVAT_MatProj;
    m_map["MatProjInv"] = eVAT_MatProjInv;
    m_map["MatView"] = eVAT_MatView;
    m_map["MatViewInv"] = eVAT_MatViewInv;
    m_map["MatModel"] = eVAT_MatModel;
    m_map["MatModelInv"] = eVAT_MatModelInv;
    m_map["MatProjView"] = eVAT_MatProjView;
    m_map["MatProjViewInv"] = eVAT_MatProjViewInv;
    m_map["MatViewModel"] = eVAT_MatViewModel;
    m_map["MatViewModelInv"] = eVAT_MatViewModelInv;
    m_map["MatProjViewModel"] = eVAT_MatProjViewModel;
    m_map["MatProjViewModelInv"] = eVAT_MatProjViewModelInv;

    m_map["MatsSkeleton"] = eVAT_MatsSkeleton;


    m_map["Texture0"] = eVAT_Texture0;
    m_map["Texture1"] = eVAT_Texture1;
    m_map["Texture2"] = eVAT_Texture2;
    m_map["Texture3"] = eVAT_Texture3;
    m_map["Texture4"] = eVAT_Texture4;
    m_map["Texture5"] = eVAT_Texture5;
    m_map["Texture6"] = eVAT_Texture6;
    m_map["Texture7"] = eVAT_Texture7;

    m_map["ViewportSize"] = eVAT_ViewportSize;
    m_map["ViewportSizeInv"] = eVAT_ViewportSizeInv;

    m_map["BinaryGradient"] = eVAT_BinaryGradient;
    m_map["BinaryGradientToScreen"] = eVAT_BinaryGradientToScreen;
    m_map["FadeInOutDist"] = eVAT_FadeInOutDist;
    m_map["FadeInOutValue"] = eVAT_FadeInOutValue;

    m_map["Blend"] = eVAT_Blend;
    m_nextID = eVAT_COUNT;

  }


  std::map<csString, csUInt32> m_map;
  csUInt32 m_nextID;
  csString m_nullString;
};


csShaderAttributeID::csShaderAttributeID(const csString &name)
  : m_name(name)
  , m_id(csShaderAttributeIDManager::Get()->Get(name))
{
}

csShaderAttributeID::csShaderAttributeID(csUInt32 id)
  : m_name(csShaderAttributeIDManager::Get()->Get(id))
  , m_id(id)
{

}

const csString &csShaderAttributeID::GetName() const
{
  return m_name;
}

csUInt32 csShaderAttributeID::GetID() const
{
  return m_id;
}



bool csShaderAttributeID::operator==(const csShaderAttributeID &other) const
{
  return m_id == other.m_id;
}
bool csShaderAttributeID::operator<(const csShaderAttributeID &other) const
{
  return m_id < other.m_id;
}

