
#pragma once

#include <vector>

class Class;
class SourceFile;
class Reader
{
public:
  Reader();

  void Read(SourceFile *sourceFile);

  size_t GetNumberOfClasses() const;
  Class *GetClass(size_t idx);

private:
  std::vector<Class*> m_classes;
};

class Containers 
{
public:
  Containers();
  bool IsCollection(const std::string &name) const;
  unsigned GetMasterTypeIdx(const std::string &name) const;

private:
  struct Container
  {
    std::string name;
    unsigned masterTypeIdx;
    Container(const std::string &name, unsigned masterTypeIdx)
      : name(name)
      , masterTypeIdx(masterTypeIdx)
    {

    }
  };
  std::vector<Container> m_collectionTypes;

};
