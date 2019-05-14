

#include <csrefl/module.hh>
#include <csrefl/class.hh>
#include <csrefl/classregistry.hh>

namespace cs
{

void Module::Initialize()
{
  cs::ClassRegistry::Get()->Register(Object::GetStaticClass());
}

}