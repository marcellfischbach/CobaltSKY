#include <cobalt/entity/blueprint/csbpentity.hh>
#include <cobalt/entity/blueprint/csbpproperty.hh>
#include <cobalt/entity/csentity.hh>

csBPEntity::csBPEntity()
  : m_entityClass(0)
{
}

void csBPEntity::SetEntityClass(const csClass *entityClass)
{
  m_entityClass = entityClass;
}

const csClass *csBPEntity::GetEntityClass() const
{
  return m_entityClass;
}

void csBPEntity::AddProperty(csBPProperty *property)
{
  m_properties.push_back(property);
}

csEntity *csBPEntity::CreateEntity() const
{
  if (!m_entityClass)
  {
    return 0;
  }
  csEntity *entity = m_entityClass->CreateInstance<csEntity>();
  if (!entity)
  {
    return 0;
  }

  for (csBPProperty *prop : m_properties)
  {
    prop->SetValue(entity);
  }
  return entity;
}

