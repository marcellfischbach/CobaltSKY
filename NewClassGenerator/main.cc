

#include <iostream>
#include <fstream>
#include <iomanip>

#include <ast.hh>
#include <cache.hh>
#include <generate/header.hh>
#include <generate/ioutput.hh>
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
  printf("usage: %s [options]\n", name);
  printf("-----------------------------------\n");
  printf("  options:\n");
  printf("    --file   <file>        a single file that should be process\n");
  printf("    --source <source>      the cc file when a single file is processed\n");
  printf("    --header <header>      the hh file when a single file is processed\n");
  printf("    --path   <path>        base path that contains the moc file and where to put the files\n");
  printf("    --prefix <prefix>      the inclue prefix where include files are located\n");
  printf("    --export <export>      <export> the name of the export api name\n");
}

class StdOutOutput : public cs::classgenerator::iOutput
{
public:
  StdOutOutput() { }

  virtual void output(const std::string& output)
  {
    std::cout << output;
  }
};

void generate(const std::string& input,
  const std::string& outputHeader,
  const std::string& outputSource,
  const std::string& exp)
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
      StdOutOutput output;
      cs::classgenerator::HeaderGenerator hg;
      hg.SetRoot(ns);
      hg.Output(outputHeader, exp, &output);
    }

    if (!outputSource.empty())
    {
      StdOutOutput output;
      cs::classgenerator::SourceGenerator sg;
      sg.SetRoot(ns);
      sg.Output(outputSource, exp, &output);
    }
    //    ns->DebugNode(0);
  }
  catch (cs::classgenerator::ParseException & e)
  {
    std::cout << "Parse Exception: " << e.what() << std::endl;
  }
}




bool compareChar(char& c1, char& c2)
{
  if (c1 == c2)
    return true;
  else if (std::toupper(c1) == std::toupper(c2))
    return true;
  return false;
}

/*
 * Case Insensitive String Comparision
 */
bool equalsCI(std::string & str1, std::string & str2)
{
  return ((str1.size() == str2.size()) &&
    std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
}


void generate_list(const std::string& path, const std::string& exp, const std::string& prefix)
{
  cs::classgenerator::Cache cache;
  cache.Load(path);

  std::ifstream stream(path + "/.csmoc");
  std::string line;
  int l = 1;
  while (std::getline(stream, line))
  {
    size_t idx = line.rfind('.');
    if (idx == std::string::npos)
    {
      continue;
    }
    std::string ext = line.substr(idx);
    if (!(equalsCI(ext, std::string(".h"))
      || equalsCI(ext, std::string(".hh"))
      || equalsCI(ext, std::string(".hpp"))
      || equalsCI(ext, std::string(".hxx"))
      )) {
      continue;
    }
    //std::cout << "Line: '" << line << "'\n";
  }

}

namespace something = cs::test;

int main(int argc, char** argv)
{
  std::string file;
  std::string source;
  std::string header;
  std::string path;
  std::string prefix;
  std::string exp;

  header = "header";
  source = "source";
  exp = "CSE_API";
  // file = "D:\\DEV\\CobaltSKY\\Engine\\cobalt\\entity\\csdynamiccolliderstate.hh";
  path = "C:\\Users\\MCEL\\CMakeBuilds\\3da2d457-a0e8-f637-864b-e8939b7c36ca\\build\\x64-Release\\Engine\\cobalt";


  for (int i = 1; i < argc; i++)
  {
    std::string arg(argv[i]);
    if (arg == "--file")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      file = std::string(argv[++i]);
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
      source = std::string(argv[++i]);
    }
    else if (arg == "--path")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      path = std::string(argv[++i]);
    }
    else if (arg == "--prefix")
    {
      if (i + 1 >= argc)
      {
        print_usage(argv[0]);
        return -1;
      }
      prefix = std::string(argv[++i]);
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
      printf("Invalid arg %s\n", arg.c_str());
      print_usage(argv[0]);
      return -1;
    }
  }

  if (!file.empty() && !source.empty() && !header.empty())
  {
    generate(file, header, source, exp);
  }
  else if (!path.empty())
  {
    generate_list(path, exp, prefix);
  }


  return 0;
}