

#include <iostream>
#include "sourcefile.hh"
#include "tokenizer.hh"

int main(int argc, char** argv)
{
  std::string fileName = argc >= 2 ? std::string(argv[1]) : std::string("D:\\DEV\\CobaltSKY\\Editor\\editormodel\\pathscanner.hh");
  

  cs::classgenerator::SourceFile sourceFile;
  sourceFile.Read(fileName);
  cs::classgenerator::Tokenizer tokenizer(sourceFile);

  std::cout << "Tokens\n";
  for (const std::string& token : tokenizer.GetTokens())
  {
    std::cout << token << std::endl;
  }

  return 0;
}