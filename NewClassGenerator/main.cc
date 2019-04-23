

#include <iostream>
#include <ast.hh>
#include <generate/header.hh>
#include <generate/source.hh>
#include <parser/sourcefile.hh>
#include <parser/token.hh>
#include <parser/tokenizer.hh>
#include <parser/parser.hh>
#include <parser/parseexception.hh>


namespace cs::test
{
class Narf
{

};
}

void print_usage(char* name)
{
  printf("usage: %s  <input-file> [options]\n", name);
  printf("-----------------------------------\n");
  printf("  <input-file>             header/source/list file\n");
  printf("  options:\n");
  printf("    --list <out>           each line in the file is one input file. <out> is the base output path\n");
  printf("    --header <header>      <header> the name of the generated header file\n");
  printf("    --source <source>      <source> the name of the generated source file\n");
  printf("    --export <export>      <export> the name of the export api name\n");
}


void generate(const std::string& input, const std::string& outputHeader, const std::string& outputSource,  const std::string &exp)
{

  cs::classgenerator::SourceFile sourceFile;
  sourceFile.Read(input);
  cs::classgenerator::Tokenizer tokenizer(sourceFile);

  cs::classgenerator::Parser parser;
  try
  {
    cs::classgenerator::ASTNode* ns = parser.Parse(tokenizer);

    if (!outputHeader.empty())
    {
      cs::classgenerator::HeaderGenerator hg;
      hg.SetRoot(ns);
      hg.Output(outputHeader, exp);
    }

    if (!outputSource.empty())
    {
      cs::classgenerator::SourceGenerator sg;
      sg.SetRoot(ns);
      sg.Output(outputSource, exp);
    }
//    ns->DebugNode(0);
  }
  catch (cs::classgenerator::ParseException & e)
  {
    std::cout << "Parse Exception: " << e.what() << std::endl;
  }


}

namespace something = cs::test;

int main(int argc, char** argv)
{
  std::string header;
  std::string source;
  std::string list;
  std::string exp;
  std::string input;

  header = "header";
  source = "source";
  exp = "CSE_API";
  input = "D:\\DEV\\CobaltSKY\\Engine\\cobalt\\entity\\csdynamiccolliderstate.hh";

  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);
    if (arg == "--list")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      list = std::string(argv[++i]);
    }
    else if (arg == "--header")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      header = std::string(argv[++i]);
    }
    else if (arg == "--source")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      list = std::string(argv[++i]);
    }
    else if (arg == "--export")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      exp = std::string(argv[++i]);
    }
    else
    {
      input = arg;
    }
  }

  /*
  if (input.empty()
    || list.empty() && source.empty() && header.empty()
    || !list.empty() && (!source.empty() || !header.empty())
    )
  {
    print_usage(argv[0]);
    return -1;
  }
  */

  if (list.empty())
  {
    generate(input, header, source, exp);
  }
  else
  {

  }


  return 0;
}