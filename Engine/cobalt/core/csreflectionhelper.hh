#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <string>


class CSE_API csReflectionPropertyHelper
{
public:
  csReflectionPropertyHelper(const cs::Class *cls, const std::string &propertyName);

  const cs::Function *GetSetter(const cs::ValueDeclaration &decl) const;
  const cs::Function *GetGetter(cs::eConstness constness) const;

private:
  std::string GetMethodName(const std::string &prefix) const;
  const cs::Property *GetProperty(const cs::Class *cls) const;

  const cs::Function*GetSetter(const cs::Class *cls, const std::string &name, const cs::ValueDeclaration&decl) const;
  const cs::Function*GetGetter(const cs::Class *cls, const std::string &name, cs::eConstness constness) const;

  const cs::Class *m_cls;
  const cs::Property*m_property;
  std::string m_propertyName;
};
