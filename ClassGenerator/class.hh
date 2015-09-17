
#ifndef __CLASS_H__
#define __CLASS_H__

#include <string>
#include <vector>
#include "sourcefile.hh"

class Property
{
public:
  Property(const std::string &typeName, const std::string &propertyName);
  bool IsValid() const;

  void Debug();

  const std::string &GetTypeName() const;
  const std::string &GetPropertyName() const;

private:

  std::string m_typeName;
  std::string m_propertyName;
};

class Class
{
public:
  Class();

  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetInterface(bool interf);
  bool IsInterface() const;

  void AddProperty(const Property &property);
  size_t GetNumberOfProperties() const;
  Property GetProperty(size_t idx) const;

  void AddSuperClass(const std::string &name);
  size_t GetNumberOfSuperClasses() const;
  const std::string &GetSuperClass(size_t idx) const;

  void Debug();
private:

  std::string m_name;

  bool m_interface;

  std::vector<Property> m_properties;
  std::vector<std::string> m_superClasses;

};


#endif /* ! __CLASS_H__ */

