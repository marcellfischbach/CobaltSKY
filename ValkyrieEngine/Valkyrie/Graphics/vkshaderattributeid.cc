
#include <Valkyrie/Graphics/ShaderID.hh>
#include <Valkyrie/vkenums.hh>
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
  : m_name(vkShaderAttributeIDManager::Get()->Get(id))
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



bool vkShaderAttributeID::operator==(const vkShaderAttributeID &other) const
{
  return m_id == other.m_id;
}
bool vkShaderAttributeID::operator<(const vkShaderAttributeID &other) const
{
  return m_id < other.m_id;
}

