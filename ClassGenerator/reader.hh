
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
