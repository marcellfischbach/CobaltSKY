
#include <cobalt/entity/csblueprint.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>


csBlueprint::csBlueprint()
  : csObject()
  , m_nextEntityID(0)
  , m_nextEntityStateID(0)
{

}


csBlueprint::~csBlueprint()
{

}

csUInt32 csBlueprint::CreateEntity(const csClass *cls, csUInt32 parentEntityID)
{
  Decl decl;
  decl.m_id = ++m_nextEntityID;
  decl.m_parentEntityID = parentEntityID;
  decl.m_class = cls;

  m_entities[decl.m_id] = decl;
  return decl.m_id;
}

csUInt32 csBlueprint::CreateEntityState(const csClass *cls, csUInt32 parentEntityID)
{
  Decl decl;
  decl.m_id = ++m_nextEntityStateID;
  decl.m_parentEntityID = parentEntityID;
  decl.m_class = cls;

  m_entityStates[decl.m_id] = decl;
  return decl.m_id;
}

void csBlueprint::SetEntityStateString(csUInt32 entityStateID, const std::string &propertyName, long idx, const std::string &value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_String;
  prop.m_string = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateBool(csUInt32 entityStateID, const std::string &propertyName, long idx, bool value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Bool;
  prop.m_bool = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateUInt8(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt8 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_UInt8;
  prop.m_uint8 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateUInt16(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt16 value)
{
  Property prop;
  prop.m_propertyIndex = idx;
  prop.m_propertyName = propertyName;
  prop.m_propertyType = ePT_UInt16;
  prop.m_uint16 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateUInt32(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt32 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_UInt32;
  prop.m_uint32 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateUInt64(csUInt32 entityStateID, const std::string &propertyName, long idx, csUInt64 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_UInt64;
  prop.m_uint64 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateInt8(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt8 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Int8;
  prop.m_int8 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateInt16(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt16 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Int16;
  prop.m_int16 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateInt32(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt32 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Int32;
  prop.m_int32 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateInt64(csUInt32 entityStateID, const std::string &propertyName, long idx, csInt64 value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Int64;
  prop.m_int64 = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateFloat(csUInt32 entityStateID, const std::string &propertyName, long idx, float value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Float;
  prop.m_float = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateDouble(csUInt32 entityStateID, const std::string &propertyName, long idx, double value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Double;
  prop.m_double = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateMatrix4f(csUInt32 entityStateID, const std::string &propertyName, long idx, const csMatrix4f &value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_Matrix4f;
  prop.m_matrix4f = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateResourceLocator(csUInt32 entityStateID, const std::string &propertyName, long idx, const csResourceLocator &value)
{
  Property prop;
  prop.m_propertyName = propertyName;
  prop.m_propertyIndex = idx;
  prop.m_propertyType = ePT_ResourceLocator;
  prop.m_locator = value;
  SetEntityStateProperty(entityStateID, prop);
}

void csBlueprint::SetEntityStateProperty(csUInt32 entityStateID, csBlueprint::Property prop)
{
  if (m_entityStates.find(entityStateID) == m_entityStates.end())
  {
    return;
  }

  m_entityStates[entityStateID].m_properties.push_back(prop);
}




iObject *csBlueprint::NewInstance() const
{
  csEntity *rootEntity;
  std::map<csUInt32, csEntity*> entities;

  for (std::map<csUInt32, Decl>::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
  {
    const Decl &decl = it->second;

    iObject *obj = decl.m_class->CreateInstance();
    csEntity *entity = csQueryClass<csEntity>(obj);
    if (entity)
    {
      entities[it->first] = entity;
      if (decl.m_parentEntityID == 0)
      {
        rootEntity = entity;
      }
    }
  }

  for (std::map<csUInt32, Decl>::const_iterator it = m_entityStates.begin(); it != m_entityStates.end(); ++it)
  {
    const Decl &decl = it->second;

    iObject *obj = decl.m_class->CreateInstance();
    csEntityState *entityState = csQueryClass<csEntityState>(obj);
    if (entityState)
    {
      for (const Property &prop : decl.m_properties)
      {
        SetEntityStatePropertyValue(entityState, prop);
      }

      csEntity *entity = entities[decl.m_parentEntityID];
      if (entity)
      {
        entity->AddState(entityState);
      }
    }
  }
  return rootEntity;
}


void csBlueprint::SetEntityStatePropertyValue(csEntityState *entityState, const csBlueprint::Property &prop) const
{
  if (!SetEntityStatePropertyValue(entityState->GetClass(), entityState, prop))
  {
    printf("%s.%s not defined\n",
      entityState->GetClass()->GetName().c_str(),
      prop.m_propertyName.c_str());
  }
}

bool csBlueprint::SetEntityStatePropertyValue(const csClass *cls, csEntityState *entityState, const csBlueprint::Property &prop) const
{
  if (!cls)
  {
    return false;
  }

  const csProperty *property = cls->GetProperty(prop.m_propertyName);
  if (!property)
  {
    for (size_t i = 0, in = cls->GetNumberOfSuperClasses(); i < in; ++i)
    {
      if (SetEntityStatePropertyValue(cls->GetSuperClass(i), entityState, prop))
      {
        return true;
      }
    }
    return false;
  }
  switch (prop.m_propertyType)
  {
  case ePT_String:
    property->Set(entityState, prop.m_string);
    break;
  case ePT_Bool:
    property->Set(entityState, prop.m_bool);
    break;
  case ePT_UInt8:
    property->Set(entityState, prop.m_uint8);
    break;
  case ePT_UInt16:
    property->Set(entityState, prop.m_uint16);
    break;
  case ePT_UInt32:
    property->Set(entityState, prop.m_uint32);
    break;
  case ePT_UInt64:
    property->Set(entityState, prop.m_uint64);
    break;
  case ePT_Int8:
    property->Set(entityState, prop.m_int8);
    break;
  case ePT_Int16:
    property->Set(entityState, prop.m_int16);
    break;
  case ePT_Int32:
    property->Set(entityState, prop.m_int32);
    break;
  case ePT_Int64:
    property->Set(entityState, prop.m_int64);
    break;
  case ePT_Float:
    property->Set(entityState, prop.m_float);
    break;
  case ePT_Double:
    property->Set(entityState, prop.m_double);
    break;
  case ePT_Matrix4f:
    property->Set(entityState, prop.m_matrix4f);
    break;
  }
  return true;
}
