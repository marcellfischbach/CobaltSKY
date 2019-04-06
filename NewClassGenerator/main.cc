

#include <iostream>
#include "codenodes.hh"
#include "sourcefile.hh"
#include "token.hh"
#include "tokenizer.hh"
#include "parser.hh"


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

  something::Narf narf;

  cs::classgenerator::Parser parser;
  cs::classgenerator::CodeNode* ns = parser.Parse(tokenizer);
  ns->DebugNode(0);

  return 0;
}