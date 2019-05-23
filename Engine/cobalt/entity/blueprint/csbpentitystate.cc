
#include <cobalt/entity/blueprint/csbpentitystate.hh>
#include <cobalt/core/property/cspropertysetter.hh>
#include <cobalt/entity/csentitystate.hh>


cs::BPEntityState::BPEntityState()
  : m_entityStateClass(0)
  , m_id(0)
  , m_parentId(0)
  , m_root(false)
{
}

cs::BPEntityState::~BPEntityState()
{
  for (auto prop : m_properties)
  {
    CS_RELEASE(prop);
  }
  m_properties.clear();

}


void cs::BPEntityState::SetId(unsigned id)
{
  m_id = id;
}

unsigned cs::BPEntityState::GetId() const
{
  return m_id;
}

void cs::BPEntityState::SetParentId(unsigned parentId)
{
  m_parentId = parentId;
}

unsigned cs::BPEntityState::GetParentId() const
{
  return m_parentId;
}

void cs::BPEntityState::SetRoot(bool root)
{
  m_root = root;
}

bool cs::BPEntityState::IsRoot() const
{
  return m_root;
}
void cs::BPEntityState::SetEntityStateClass(const cs::Class *entityStateClass)
{
  m_entityStateClass = entityStateClass;
}

const cs::Class *cs::BPEntityState::GetEntityStateClass() const
{
  return m_entityStateClass;
}

void cs::BPEntityState::AddProperty(cs::PropertySetter *property)
{
  CS_ADDREF(property);
  m_properties.push_back(property);
}

cs::EntityState *cs::BPEntityState::CreateEntityState() const
{
  if (!m_entityStateClass)
  {
    return 0;
  }

  cs::EntityState *state =  m_entityStateClass->CreateInstance<cs::EntityState>();
  if (!state)
  {
    return 0;
  }
  state->SetId(GetId());

  for (cs::PropertySetter *prop : m_properties)
  {
    prop->Apply(state);
  }

  return state;
}

