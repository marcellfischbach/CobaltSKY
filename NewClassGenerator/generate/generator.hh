
#pragma once

#include <list>
#include <string>
#include <vector>

namespace cs::classgenerator
{




class ASTNode;
class ClassNode;
class NamespaceNode;

class Generator
{
public:
  Generator();

  void SetRoot(ASTNode* node);


  std::vector<ClassNode*> FindAllMajorClasses();
  std::list<NamespaceNode*> GetAllNamespaces(ASTNode* node);

protected:
  void ScanClasses(ASTNode* node, std::vector<ClassNode*>& classes);

  
  ASTNode* m_root;
};


}