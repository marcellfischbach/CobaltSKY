
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/entity/csentitystate.hh>


csBPEntityState::csBPEntityState()
  : m_entityStateClass(0)
  , m_id(0)
  , m_parentId(0)
  , m_root(false)
{
}

csBPEntityState::~csBPEntityState()
{
  for (auto prop : m_properties)
  {
    CS_RELEASE(prop);
  }
  m_properties.clear();

}


void csBPEntityState::SetId(unsigned id)
{
  m_id = id;
}

unsigned csBPEntityState::GetId() const
{
  return m_id;
}

void csBPEntityState::SetParentId(unsigned parentId)
{
  m_parentId = parentId;
}

unsigned csBPEntityState::GetParentId() const
{
  return m_parentId;
}

void csBPEntityState::SetRoot(bool root)
{
  m_root = root;
}

bool csBPEntityState::IsRoot() const
{
  return m_root;
}
void csBPEntityState::SetEntityStateClass(const csClass *entityStateClass)
{
  m_entityStateClass = entityStateClass;
}

const csClass *csBPEntityState::GetEntityStateClass() const
{
  return m_entityStateClass;
}

void csBPEntityState::AddProperty(csPropertySetter *property)
{
  CS_ADDREF(property);
  m_properties.push_back(property);
}

csEntityState *csBPEntityState::CreateEntityState() const
{
  if (!m_entityStateClass)
  {
    return 0;
  }

  csEntityState *state =  m_entityStateClass->CreateInstance<csEntityState>();
  if (!state)
  {
    return 0;
  }
  state->SetId(GetId());

  for (csPropertySetter *prop : m_properties)
  {
    prop->Apply(state);
  }

  return state;
}

