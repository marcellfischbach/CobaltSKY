
#ifndef __CLASS_H__
#define __CLASS_H__

#include <map>
#include <string>
#include <vector>
#include "sourcefile.hh"

enum TypeSpecifiction
{
  eTS_Value,
  eTS_Reference,
  eTS_Pointer,
  eTS_PointerToPointer,
};

class Property
{
public:
  Property(bool isConst, const std::string &typeName, TypeSpecifiction typeSpecification, const std::string &propertyName, const std::map<std::string, std::string> &meta);
  bool IsValid() const;

  void Debug();

  bool IsConst() const;
  const std::string &GetTypeName() const;
  TypeSpecifiction GetTypeSpecification() const;
  const std::string &GetPropertyName() const;
  const std::string &GetPropertyVariableName() const;

  const std::string GetGetter() const;
  const std::string GetSetter() const;

  const std::map<std::string, std::string> &GetMeta () const;

private:
  bool m_const;
  std::string m_typeName;
  TypeSpecifiction m_typeSpecification;
  std::string m_propertyName;
  std::map<std::string, std::string> m_meta;
};

class Function
{
public:
  struct Parameter
  {
    std::string type;
    std::string name;
    bool isConst;
    TypeSpecifiction typeSpecifiction;
  };

  Function();

  void SetName(const std::string &name);
  const std::string &GetName() const;

  void SetConst(bool isConst);
  bool IsConst() const;

  void SetVirtual(bool isVirtual);
  bool IsVirtual() const;

  void SetOverride(bool isOverride);
  bool IsOverride() const;

  bool IsValid() const { return m_valid; }

  void Debug();


  void SetReturnType(const std::string &type, bool isConst, TypeSpecifiction typeSpecifiction);
  const std::string &GetReturnType() const;
  bool IsReturnConst() const;
  TypeSpecifiction GetReturnTypeSpecification() const;

  void AddParameter(const std::string &name, const std::string &type, bool isConst, TypeSpecifiction typeSpecifiction);
  size_t GetNumberOfParameters() const;
  Parameter GetParameter(size_t idx) const;

private:

  bool m_valid;

  bool m_virtual;
  std::string m_functionName;
  bool m_const;
  bool m_override;

  std::string m_returnType;
  bool m_returnTypeIsConst;
  TypeSpecifiction m_returnTypeSpecifiction;

  std::vector<Parameter> m_parameters;
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

  void AddFunction(const Function &function);
  size_t GetNumberOfFunctions() const;
  Function GetFunction(size_t idx) const;

  void AddSuperClass(const std::string &name);
  size_t GetNumberOfSuperClasses() const;
  const std::string &GetSuperClass(size_t idx) const;

  void Debug();
private:

  std::string m_name;

  bool m_interface;

  std::vector<Property> m_properties;
  std::vector<Function> m_functions;
  std::vector<std::string> m_superClasses;

};


#endif /* ! __CLASS_H__ */

