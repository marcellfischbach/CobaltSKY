
#include <generate/generator.hh>
#include <ast.hh>



namespace cs::classgenerator
{


Generator::Generator()
  : m_root(nullptr)
{

}

void Generator::SetRoot(ASTNode* root)
{
  m_root = root;
}

std::vector<ClassNode*> Generator::FindAllMajorClasses()
{

  std::vector<ClassNode*> classes;

  ScanClasses(m_root, classes);

  return classes;
}

void Generator::ScanClasses(ASTNode* node, std::vector<ClassNode*>& classes)
{
  if (node->GetType() == eANT_Class)
  {
    classes.push_back(static_cast<ClassNode*>(node));
  }
  else
  {
    for (auto child : node->GetChildren())
    {
      ScanClasses(child, classes);
    }
  }
}


std::list<NamespaceNode*> Generator::GetAllNamespaces(ASTNode* node)
{
  std::list<NamespaceNode*> ns;
  while (node)
  {
    if (node->GetType() == eANT_Namespace)
    {
      ns.push_front(static_cast<NamespaceNode*>(node));
    }
    node = node->GetParent();
  }
  return ns;
}



}