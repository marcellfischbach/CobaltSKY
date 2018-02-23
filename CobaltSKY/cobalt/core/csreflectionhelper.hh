#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <string>


class CSE_API csReflectionPropertyHelper
{
public:
  csReflectionPropertyHelper(const csClass *cls, const std::string &propertyName);

  const csFunction *GetSetter(const csValueDeclaration &decl) const;
  const csFunction *GetGetter(csConstness constness) const;

private:
  std::string GetMethodName(const std::string &prefix) const;
  const csProperty *GetProperty(const csClass *cls) const;

  const csFunction *GetSetter(const csClass *cls, const std::string &name, const csValueDeclaration &decl) const;
  const csFunction *GetGetter(const csClass *cls, const std::string &name, csConstness constness) const;

  const csClass *m_cls;
  const csProperty *m_property;
  std::string m_propertyName;
};
