#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <map>

#include <cobalt/entity/csblueprint.refl.hh>

class csEntityState;

CS_CLASS()
class CSE_API csBlueprint : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csBlueprint();
  virtual ~csBlueprint();

  iObject *NewInstance() const;

  csUInt32 CreateEntity(const csClass *cls, csUInt32 parentEntityID = 0);
  csUInt32 CreateEntityState(const csClass *cls, csUInt32 parentEntityID);

  void SetEntityStateString(csUInt32 entityStateID, const std::string &propertyName, const std::string &value);
  void SetEntityStateBool(csUInt32 entityStateID, const std::string &propertyName, bool value);
  void SetEntityStateUInt8(csUInt32 entityStateID, const std::string &propertyName, csUInt8 value);
  void SetEntityStateUInt16(csUInt32 entityStateID, const std::string &propertyName, csUInt16 value);
  void SetEntityStateUInt32(csUInt32 entityStateID, const std::string &propertyName, csUInt32 value);
  void SetEntityStateUInt64(csUInt32 entityStateID, const std::string &propertyName, csUInt64 value);
  void SetEntityStateInt8(csUInt32 entityStateID, const std::string &propertyName, csInt8 value);
  void SetEntityStateInt16(csUInt32 entityStateID, const std::string &propertyName, csInt16 value);
  void SetEntityStateInt32(csUInt32 entityStateID, const std::string &propertyName, csInt32 value);
  void SetEntityStateInt64(csUInt32 entityStateID, const std::string &propertyName, csInt64 value);
  void SetEntityStateFloat(csUInt32 entityStateID, const std::string &propertyName, float value);
  void SetEntityStateDouble(csUInt32 entityStateID, const std::string &propertyName, double value);
  void SetEntityStateMatrix4f(csUInt32 entityStateID, const std::string &propertyName, const csMatrix4f &value);

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
    ePT_Matrix4f,
  };

  struct Property
  {
    std::string m_propertyName;
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
    csMatrix4f m_matrix4f;
  };

  struct Decl
  {
    csUInt32 m_id;
    csUInt32 m_parentEntityID;
    const csClass *m_class;
    std::vector<Property> m_properties;
  };

  void SetEntityStateProperty(csUInt32 entityStateID, Property prop);
  void SetEntityStatePropertyValue(csEntityState *entityState, const Property &prop) const;
  bool SetEntityStatePropertyValue(const csClass *cls, csEntityState *entityState, const Property &prop) const;

  std::map<csUInt32, Decl> m_entities;
  std::map<csUInt32, Decl> m_entityStates;

  csUInt32 m_nextEntityID;
  csUInt32 m_nextEntityStateID;



};

