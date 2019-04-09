

#include <iostream>
#include <ast.hh>
#include <parser/sourcefile.hh>
#include <parser/token.hh>
#include <parser/tokenizer.hh>
#include <parser/parser.hh>


namespace cs::test
{
class Narf
{

};
}

namespace something = cs::test;

int main(int argc, char** argv)
{
  std::string fileName = argc >= 2 ? std::string(argv[1]) : std::string("D:\\DEV\\CobaltSKY\\Editor\\editormodel\\pathscanner.hh");
  

  cs::classgenerator::SourceFile sourceFile;
  sourceFile.Read(fileName);
  cs::classgenerator::Tokenizer tokenizer(sourceFile);

  std::cout << "Tokens\n";
  for (const cs::classgenerator::Token& token : tokenizer.GetTokens())
  {
    std::cout << token.Get() << std::endl;
  }


  cs::classgenerator::Parser parser;
  cs::classgenerator::ASTNode* ns = parser.Parse(tokenizer);
  ns->DebugNode(0);

  return 0;
}