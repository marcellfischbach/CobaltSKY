#pragma once
/*
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <map>

#include <cobalt/entity/csblueprint.refl.hh>

class cs::EntityState;

CS_CLASS()
class CSE_API cs::Blueprint : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  cs::Blueprint();
  virtual ~cs::Blueprint();

  cs::iObject *NewInstance() const;

  csUInt32 CreateEntity(const cs::Class *cls, csUInt32 parentEntityID = 0);
  csUInt32 CreateEntityState(const cs::Class *cls, csUInt32 parentEntityID, csUInt32 parentStateID = 0);

  void SetEntityStateString(csUInt32 entityStateID, const std::string &propertyName, long idx, const std::string &value);
  void SetEntityStateBool(csUInt32 entityStateID, const std::string &propertyName, long idx, bool value);
  void SetEntityStateUInt8(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt8 value);
  void SetEntityStateUInt16(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt16 value);
  void SetEntityStateUInt32(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt32 value);
  void SetEntityStateUInt64(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt64 value);
  void SetEntityStateInt8(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt8 value);
  void SetEntityStateInt16(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt16 value);
  void SetEntityStateInt32(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt32 value);
  void SetEntityStateInt64(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt64 value);
  void SetEntityStateFloat(csUInt32 entityStateID, const std::string &propertyName, long idx, float value);
  void SetEntityStateDouble(csUInt32 entityStateID, const std::string &propertyName, long idx, double value);
  void SetEntityStateVector2f(csUInt32 entityStateID, const std::string &propertyName, long idx, const cs::Vector3f &value);
  void SetEntityStateVector3f(csUInt32 entityStateID, const std::string &propertyName, long idx, const cs::Vector3f &value);
  void SetEntityStateVector4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const cs::Vector4f &value);
  void SetEntityStateColor4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const cs::Color4f &value);
  void SetEntityStateMatrix4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const cs::Matrix4f &value);
  void SetEntityStateResourceLocator(csUInt32 entityStateID, const std::string &propertyName, long idx, const cs::ResourceLocator &value);

private:

  enum PropertyType
  {
    cs::ePT_String,
    cs::ePT_Bool,
    cs::ePT_UInt8,
    cs::ePT_UInt16,
    cs::ePT_UInt32,
    cs::ePT_UInt64,
    cs::ePT_Int8,
    cs::ePT_Int16,
    cs::ePT_Int32,
    cs::ePT_Int64,
    cs::ePT_Float,
    cs::ePT_Double,
    cs::ePT_Vector2f,
    cs::ePT_Vector3f,
    cs::ePT_Vector4f,
    cs::ePT_Color4f,
    cs::ePT_Matrix4f,
    cs::ePT_ResourceLocator,
  };

  struct Property
  {
    std::string m_propertyName;
    long m_propertyIndex;
    PropertyType m_propertyType;
    std::string m_string;
    bool m_bool;
    csUInt8 m_uint8;
    csUInt16 m_uint16;
    csUInt32 m_uint32;
    csUInt64 m_uint64;
    csInt8 m_int8;
    csInt16 m_int16;
    csInt32 m_int32;
    csInt64 m_int64;
    float m_float;
    double m_double;
    cs::Vector2f m_vector2f;
    cs::Vector3f m_vector3f;
    cs::Vector4f m_vector4f;
    cs::Color4f m_color4f;
    cs::Matrix4f m_matrix4f;
    cs::ResourceLocator m_resourceLocator;
  };



  struct Decl
  {
    csUInt32 m_id;
    csUInt32 m_parentEntityID;
    const cs::Class *m_class;
    std::vector<Property> m_properties;
  };

  void SetEntityStateProperty(csUInt32 entityStateID, Property prop);
  void SetEntityStatePropertyValue(cs::EntityState *entityState, const Property &prop) const;
  bool SetEntityStatePropertyValue(const cs::Class *cls, cs::EntityState *entityState, const Property &prop) const;
  const cs::Function *FindFunction(const cs::Class *cls, const std::string &name, const std::string &typeName) const;

  std::map<csUInt32, Decl> m_entities;
  std::map<csUInt32, Decl> m_entityStates;

  csUInt32 m_nextEntityID;
  csUInt32 m_nextEntityStateID;



};

*/