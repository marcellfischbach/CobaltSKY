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

class csEntityState;

CS_CLASS()
class CSE_API csBlueprint : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  csBlueprint();
  virtual ~csBlueprint();

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
  void SetEntityStateVector2f(csUInt32 entityStateID, const std::string &propertyName, long idx, const csVector3f &value);
  void SetEntityStateVector3f(csUInt32 entityStateID, const std::string &propertyName, long idx, const csVector3f &value);
  void SetEntityStateVector4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const csVector4f &value);
  void SetEntityStateColor4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const csColor4f &value);
  void SetEntityStateMatrix4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const csMatrix4f &value);
  void SetEntityStateResourceLocator(csUInt32 entityStateID, const std::string &propertyName, long idx, const csResourceLocator &value);

private:

  enum PropertyType
  {
    ePT_String,
    ePT_Bool,
    ePT_UInt8,
    ePT_UInt16,
    ePT_UInt32,
    ePT_UInt64,
    ePT_Int8,
    ePT_Int16,
    ePT_Int32,
    ePT_Int64,
    ePT_Float,
    ePT_Double,
    ePT_Vector2f,
    ePT_Vector3f,
    ePT_Vector4f,
    ePT_Color4f,
    ePT_Matrix4f,
    ePT_ResourceLocator,
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
    csVector2f m_vector2f;
    csVector3f m_vector3f;
    csVector4f m_vector4f;
    csColor4f m_color4f;
    csMatrix4f m_matrix4f;
    csResourceLocator m_resourceLocator;
  };



  struct Decl
  {
    csUInt32 m_id;
    csUInt32 m_parentEntityID;
    const cs::Class *m_class;
    std::vector<Property> m_properties;
  };

  void SetEntityStateProperty(csUInt32 entityStateID, Property prop);
  void SetEntityStatePropertyValue(csEntityState *entityState, const Property &prop) const;
  bool SetEntityStatePropertyValue(const cs::Class *cls, csEntityState *entityState, const Property &prop) const;
  const cs::Function *FindFunction(const cs::Class *cls, const std::string &name, const std::string &typeName) const;

  std::map<csUInt32, Decl> m_entities;
  std::map<csUInt32, Decl> m_entityStates;

  csUInt32 m_nextEntityID;
  csUInt32 m_nextEntityStateID;



};

*/