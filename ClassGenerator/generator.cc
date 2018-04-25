
#include <stdio.h>
#include <string>
#include <vector>
#include "sourcefile.hh"
#include "reader.hh"
#include "cache.hh"
#include "class.hh"
#include <ShlObj.h>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
static void replaceSlash(std::string &path)
{
  for (size_t i = 0, in = path.length(); i < in; ++i)
  {
    if (path[i] == '\\')
    {
      path[i] = '/';
    }
  }
}


std::string Convert(TypeSpecifiction ts)
{
  switch (ts)
  {
  case eTS_Value:
    return "eVMM_Value";
  case eTS_Reference:
    return "eVMM_Reference";
  case eTS_Pointer:
    return "eVMM_Pointer";
  case eTS_PointerToPointer:
    return "eVMM_PointerToPointer";
  }
  return "eVMM_Value";
}


static bool getOutputFilenames(const std::string &in_filename, const std::string &in_outputDirectory, std::string &out_headerName, std::string &out_sourceName, bool simple)
{
  size_t dotPos = in_filename.find_last_of('.');
  if (dotPos == std::string::npos)
  {
    return false;
  }
  std::string ext = in_filename.substr(dotPos);
  if (ext != ".h" && ext != ".hh" && ext != ".hpp")
  {
    return false;
  }

  std::string filename = std::string(in_filename);
  replaceSlash(filename);
  size_t posDot = filename.find_last_of('.');


  std::string name = filename.substr(0, posDot);


  std::string path = simple ? name : (in_outputDirectory + '/' + name);






  // now those two vars contains the final resulting files
  out_headerName = path + ".refl.hh";
  out_sourceName = path + ".refl.cc";

  return true;
}

static std::string CreateHeaderFile(Class *clazz, const std::string &api)
{
  std::string className = clazz->GetName() + std::string("Class");
  std::string result = "";
  result += "\n";
  result += "\n";
  result += "class " + api + " " + className + " : public csClass\n";
  result += "{\n";
  result += "public:\n";
  result += "  " + className + "();\n";
  result += "  \n";
  result += "  static " + className + " *Get();\n";
  result += "  \n";
  result += "  virtual iObject *CreateInstance() const;\n";
  result += "  \n";
  result += "  \n";
  result += "};\n";
  result += "  \n";
  result += "  \n";
  return result;
}

std::string GetCodeForAttrParam(TypeSpecifiction ts)
{
  std::string res = "";
  switch (ts)
  {
  case eTS_Value:
  case eTS_Reference:
    return "*";
    break;
  case eTS_Pointer:
    res = "**";
    break;
  case eTS_PointerToPointer:
    res = "***";
    break;
  }
  return res;
}

std::string GetCodeForAttrPtr(TypeSpecifiction ts)
{
  std::string res = "";
  switch (ts)
  {
  case eTS_Value:
    return "*";
    break;
  case eTS_Reference:
  case eTS_Pointer:
    res = "**";
    break;
  case eTS_PointerToPointer:
    res = "***";
    break;
  }
  return res;
}

std::string GetCodeForAttrResult(TypeSpecifiction ts)
{
  std::string res = "";
  switch (ts)
  {
  case eTS_Value:
    break;
  case eTS_Reference:
    res = "&";
    break;
  case eTS_Pointer:
    res = "*";
    break;
  case eTS_PointerToPointer:
    res = "**";
    break;
  }
  return res;
}

std::string ConvertVirtual(bool v)
{
  return v ? std::string("eFV_Virtual") : std::string("eFV_NonVirtual");
}

std::string ConvertConstness(bool v)
{
  return v ? std::string("eC_Const") : std::string("eC_NonConst");
}

/*
std::string Convert(bool v)
{
  return v ? std::string("true") : std::string("false");
}
*/

static std::string CreateProperty(const std::string &className, Class *clazz, Property &prop, const std::string &api, std::vector<std::string> &propertyClasses)
{
  std::string propClassName = className + prop.GetPropertyName();
  propertyClasses.push_back(propClassName);
  std::string propGetter = prop.GetGetter();
  std::string propSetter = prop.GetSetter();
  std::string result = "";
  result += "class " + api + " " + propClassName + " : public csProperty\n";
  result += "{\n";
  result += "public:\n";
  result += "  " + propClassName + "() \n";
  result += "    : csProperty (csValueDeclaration(eC_NonConst, \"" + prop.GetContainerTypeName() + "\", eVMM_Value), \"" + prop.GetPropertyName() + "\", csValueDeclaration(eC_NonConst, \"" + prop.GetTypeName() + "\", " + Convert(prop.GetTypeSpecification())+ "))\n";
  result += "  {\n";
  for (auto it : prop.GetMeta())
  {
    result += "    SetProperty (\"" + it.first + "\", \"" + it.second + "\");\n";
  }
  result += "  }\n";
  result += "  \n";
  result += "  virtual void SetValue(iObject *object, void *data) const\n";
  result += "  {\n";
  /*
  if (!propSetter.empty())
  {
    result += "    " + clazz->GetName() + " *d = csQueryClass<" + clazz->GetName() + ">(object);\n";
    result += "    if (d)\n";
    result += "    {\n";
    result += "      " + prop.GetTypeName() + " &v = *reinterpret_cast<" + prop.GetTypeName() + "*>(data);\n";
    result += "      d->Set" + prop.GetPropertyName() + "(v);\n";
    result += "    }\n";
  }
  */
  result += "  }\n";
  result += "  \n";
  result += "  virtual const void *GetValue(const iObject *object) const\n";
  result += "  {\n";
  /*
  if (!propGetter.empty())
  {
    result += "    const " + clazz->GetName() + " *d = csQueryClass<" + clazz->GetName() + ">(object);\n";
    result += "    if (!d) return 0;\n";
    result += "    const " + prop.GetTypeName() + " v = d->" + propGetter + "();\n";
    result += "    return reinterpret_cast<const void*>(&v);\n";
  }
  else
  */
  {
    result += "    return 0;\n";
  }
  result += "  }\n";
  result += "  \n";
  result += "};\n";
  result += "\n";

  return result;
}

static std::string CreateFunction(const std::string &className, Class *clazz, Function &func, const std::string &api, std::vector<std::string> &functionClasses)
{
  std::string funcClassName = className + func.GetName();
  if (func.IsConst())
  {
    funcClassName += "Const";
  }
  functionClasses.push_back(funcClassName);
  std::string result = "";
  result += "class " + api + " " + funcClassName + " : public csFunction\n";
  result += "{\n";
  result += "public:\n";
  result += "  " + funcClassName + "() \n";
  result += "    : csFunction (" + ConvertVirtual(func.IsVirtual()) + std::string(", ");
  result += /*                  */ std::string("csValueDeclaration(") + ConvertConstness(func.IsReturnConst()) + std::string(", ");
  result += /*                                                       */ std::string("\"") + func.GetReturnType() + std::string("\", ");
  result += /*                                                       */ Convert(func.GetReturnTypeSpecification()) + std::string("), ");
  result += /*                  */ std::string("\"") + func.GetName() + std::string("\", ");
  result += /*                  */ ConvertConstness(func.IsConst()) + ")\n";
  result += "  {\n";
  for (size_t i = 0, in = func.GetNumberOfParameters(); i < in; ++i)
  {
    Function::Parameter param = func.GetParameter(i);
    result += "    AddAttribute(csFunctionAttribute(csValueDeclaration(" + ConvertConstness(param.isConst) + std::string(", ");
    result += /*                                                        */ std::string("\"") + param.type + std::string("\", ");
    result += /*                                                        */ Convert(param.typeSpecifiction) + std::string("), ");
    result += /*                                 */ "\"" + param.name + "\"));\n";
  }
  result += "  }\n";
  result += "  \n";
  result += "  virtual void Invoke(iObject* obj, ...) const\n";
  result += "  {\n";
  result += "    " + clazz->GetName() + " *d = csQueryClass<" + clazz->GetName() + ">(obj);\n";
  result += "    if (!d) return;\n";
  result += "    va_list lst;\n";
  result += "    va_start (lst, obj);\n";
  for (size_t i = 0, in = func.GetNumberOfParameters(); i < in; ++i)
  {
    Function::Parameter param = func.GetParameter(i);
    std::string typeStr = (param.isConst ? "const " : "") + param.type + " " + GetCodeForAttrParam(param.typeSpecifiction);
    result += "    " + typeStr + param.name + " = va_arg(lst, " + typeStr + ");\n";
  }
  bool hasReturnValue = func.GetReturnType() != std::string("void");
  if (hasReturnValue)
  {
    std::string typeStr = (func.IsReturnConst() ? "const " : "") + func.GetReturnType() + " " + GetCodeForAttrPtr(func.GetReturnTypeSpecification());
    std::string resStr = (func.IsReturnConst() ? "const " : "") + func.GetReturnType() + " " + GetCodeForAttrResult(func.GetReturnTypeSpecification());
    result += "    " + typeStr + "___ptr___result___ = va_arg(lst, " + typeStr + ");\n";
    result += "    va_end (lst);\n";
    result += "    " + resStr + " ___value___result___ = ";
  }
  else
  {
    result += "    va_end (lst);\n";
    result += "    ";
  }
  result += "d->" + func.GetName() + "(";
  for (size_t i = 0, in = func.GetNumberOfParameters(); i < in; ++i)
  {
    Function::Parameter param = func.GetParameter(i);
    result += "*" + param.name;
    if (i + 1 < in)
    {
      result += ", ";
    }
  }
  result += ");\n";
  if (hasReturnValue)
  {
    result += "    if (___ptr___result___)\n";
    result += "    {\n";
    switch (func.GetReturnTypeSpecification())
    {
    case eTS_Value:
      result += "      *___ptr___result___ = ___value___result___; \n";
      break;
    case eTS_Reference:
      result += "      *___ptr___result___ = &___value___result___; \n";
      break;
    case eTS_Pointer:
      result += "      *___ptr___result___ = ___value___result___; \n";
      break;
    }
    result += "    }\n";
  }

  result += "  }\n";
  result += "  virtual void Invoke(const iObject* obj, ...) const\n";
  result += "  {\n";
  if (func.IsConst())
  {
    result += "    const " + clazz->GetName() + " *d = csQueryClass<const " + clazz->GetName() + ">(obj);\n";
    result += "    if (!d) return;\n";
    result += "    va_list lst;\n";
    result += "    va_start (lst, obj);\n";
    for (size_t i = 0, in = func.GetNumberOfParameters(); i < in; ++i)
    {
      Function::Parameter param = func.GetParameter(i);
      std::string typeStr = (param.isConst ? "const " : "") + param.type + " " + GetCodeForAttrParam(param.typeSpecifiction);
      result += "    " + typeStr + param.name + " = va_arg(lst, " + typeStr + ");\n";
    }
    bool hasReturnValue = func.GetReturnType() != std::string("void");
    if (hasReturnValue)
    {
      std::string typeStr = (func.IsReturnConst() ? "const " : "") + func.GetReturnType() + " " + GetCodeForAttrPtr(func.GetReturnTypeSpecification());
      std::string resStr = (func.IsReturnConst() ? "const " : "") + func.GetReturnType() + " " + GetCodeForAttrResult(func.GetReturnTypeSpecification());
      result += "    " + typeStr + "___ptr___result___ = va_arg(lst, " + typeStr + ");\n";
      result += "    va_end (lst);\n";
      result += "    " + resStr + " ___value___result___ = ";
    }
    else
    {
      result += "    va_end (lst);\n";
      result += "    ";
    }
    result += "d->" + func.GetName() + "(";
    for (size_t i = 0, in = func.GetNumberOfParameters(); i < in; ++i)
    {
      Function::Parameter param = func.GetParameter(i);
      result += "*" + param.name;
      if (i + 1 < in)
      {
        result += ", ";
      }
    }
    result += ");\n";
    if (hasReturnValue)
    {
      result += "    if (___ptr___result___)\n";
      result += "    {\n";
      switch (func.GetReturnTypeSpecification())
      {
      case eTS_Value:
        result += "      *___ptr___result___ = ___value___result___; \n";
        break;
      case eTS_Reference:
        result += "      *___ptr___result___ = &___value___result___; \n";
        break;
      case eTS_Pointer:
        result += "      *___ptr___result___ = ___value___result___; \n";
        break;
      }
      result += "    }\n";
    }
  }
  result += "  }\n";
  result += "};\n";
  result += "\n";

  return result;
}


static std::string CreateSourceFile(Class *clazz, const std::string &api)
{
  std::string className = clazz->GetName() + std::string("Class");
  std::string result = "";
  result += "\n";
  result += "\n";


  std::vector<std::string> propertyClasses;
  for (size_t i = 0, in = clazz->GetNumberOfProperties(); i < in; ++i)
  {
    Property prop = clazz->GetProperty(i);
    result += CreateProperty(className, clazz, prop, api, propertyClasses);
  }

  std::vector<std::string> functionClasses;
  for (size_t i = 0, in = clazz->GetNumberOfFunctions(); i < in; ++i)
  {
    Function func = clazz->GetFunction(i);
    result += CreateFunction(className, clazz, func, api, functionClasses);
  }

  result += className + " *" + className + "::Get()\n";
  result += "{\n";
  result += "  static " + className + " static_class;\n";
  result += "  return &static_class;\n";
  result += "}\n";
  result += "\n";
  result += className + "::" + className + "()\n";
  result += "  : csClass(\"" + clazz->GetName() + "\")\n";
  result += "{\n";
  for (size_t i = 0, in = clazz->GetNumberOfSuperClasses(); i < in; ++i)
  {
    std::string super = clazz->GetSuperClass(i);
    result += "  AddSuperClass(" + super + "Class::Get());\n";
  }

  for (auto propName : propertyClasses)
  {
    result += "  AddProperty(new " + propName + "());\n";
  }
  for (auto funcName : functionClasses)
  {
    result += "  AddFunction(new " + funcName + "());\n";
  }
  result += "}\n";
  result += "\n";
  result += "iObject *" + className + "::CreateInstance() const\n";
  result += "{\n";
  if (clazz->IsInterface())
  {
    result += "  return 0;\n";
  }
  else
  {
    result += "  return static_cast<iObject*>(new " + clazz->GetName() + "());\n";
  }
  result += "}\n";
  result += "\n";
  result += "const csClass *" + clazz->GetName() + "::GetClass () const\n";
  result += "{\n";
  result += "  return " + className + "::Get();\n";
  result += "}\n";
  result += "\n";
  result += "const csClass *" + clazz->GetName() + "::GetStaticClass ()\n";
  result += "{\n";
  result += "  return " + className + "::Get();\n";
  result += "}\n";
  result += "\n";
  result += "void *" + clazz->GetName() + "::QueryClass(const csClass* clazz) \n";
  result += "{\n";
  result += "  if (clazz == " + className + "::Get ())\n";
  result += "  {\n";
  result += "    return static_cast<" + clazz->GetName() + "*>(this);\n";
  result += "  }\n";
  if (clazz->GetNumberOfSuperClasses() > 0)
  {
    result += "  void *super = 0;\n";
    for (size_t s = 0, sn = clazz->GetNumberOfSuperClasses(); s < sn; ++s)
    {
      std::string super = clazz->GetSuperClass(s);
      result += "  super = " + super + "::QueryClass(clazz);\n";
      result += "  if (super != 0)\n";
      result += "  {\n";
      result += "    return super;\n";
      result += "  }\n";
    }
  }
  result += "  return 0;\n";
  result += "}\n";
  result += "\n";
  result += "\n";
  result += "const void *" + clazz->GetName() + "::QueryClass(const csClass* clazz) const\n";
  result += "{\n";
  result += "  if (clazz == " + className + "::Get ())\n";
  result += "  {\n";
  result += "    return static_cast<const " + clazz->GetName() + "*>(this);\n";
  result += "  }\n";
  if (clazz->GetNumberOfSuperClasses() > 0)
  {
    result += "  const void *super = 0;\n";
    for (size_t s = 0, sn = clazz->GetNumberOfSuperClasses(); s < sn; ++s)
    {
      result += "  super = " + clazz->GetSuperClass(s) + "::QueryClass(clazz);\n";
      result += "  if (super != 0)\n";
      result += "  {\n";
      result += "    return super;\n";
      result += "  }\n";
    }
  }
  result += "  return 0;\n";
  result += "}\n";
  result += "\n";
  result += "\n";


  return result;
}

/*
class Test
{
public:
  Test() :m_v(0) { } // std::cout << "Test::ctor" << std::endl;;

  Test(const Test &test) : m_v(test.m_v) { std::cout << "Test::copy-ctor" << std::endl; }
  Test(Test &&test) : m_v(test.m_v) { std::cout << "Test::move-ctor" << std::endl; }

  Test &operator= (const Test& other)
  {
    m_v = other.m_v;
    return *this;
  }

  void setV(int v) { m_v = v; }
  int getV() const { return m_v; }

private:
  int m_v;
};

Test g_test;

void performMyTestVoid(Test &test)
{
  std::cout << "Test: 0x" << &test << std::endl;;
}

const Test performMyTestValue()
{
  return g_test;
}

const Test &performMyTestReference()
{
  return g_test;
}

const Test *performMyTestPointer()
{
  return &g_test;
}



void performMyTestVoidRefl(int n, ...)
{
  va_list lst;
  va_start(lst, n);
  Test *parg = va_arg(lst, Test *);
  va_end(lst);


  performMyTestVoid(*parg);
}

void performMyTestValueRefl(int n, ...)
{
  va_list lst;
  va_start(lst, n);
  Test *pres = va_arg(lst, Test *);
  va_end(lst);


  const Test result = performMyTestReference();
  if (pres)
  {
    *pres = result;
  }
}


void performMyTestReferenceRefl(int n, ...)
{
  va_list lst;
  va_start(lst, n);
  const Test **pres = va_arg(lst, const Test **);
  va_end(lst);


  const Test &result = performMyTestReference();
  if (pres)
  {
    *pres = &result;
  }
}

void performMyTestPointerRefl(int n, ...)
{
  va_list lst;
  va_start(lst, n);
  const Test **pres = va_arg(lst, const Test **);
  va_end(lst);


  const Test *result = performMyTestPointer();
  if (pres)
  {
    *pres = result;
  }
}

template<typename A0>
void _performMyTestVoid(A0 a0)
{
  performMyTestVoidRefl(0, &a0);
}


template<typename R>
R performMyTestValue()
{
  R r;
  performMyTestValueRefl(0, &r);
  return r;
}

template<typename R>
R &performMyTestReference()
{
  R *r;
  performMyTestReferenceRefl(0, &r);
  return *r;
}

template<typename R>
R *performMyTestPointer()
{
  R *r;
  performMyTestPointerRefl(0, &r);
  return r;
}

int test(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cout << "Usage " << std::string(argv[0]) << " --test <headerfile> <outputname>" << std::endl;
    return -1;
  }

  std::string inputHeaderFilename = std::string(argv[2]);

  SourceFile *sourceFile = new SourceFile(inputHeaderFilename);
  Reader *reader = new Reader();
  reader->Read(sourceFile);

  std::string fileNameHeaderOutput = std::string(argv[3]) + std::string(".hh");
  std::string fileNameSourceOutput = std::string(argv[3]) + std::string(".cc");
  FILE *headerOutput;
  fopen_s(&headerOutput, fileNameHeaderOutput.c_str(), "wt");
  FILE *sourceOutput;
  fopen_s(&sourceOutput, fileNameSourceOutput.c_str(), "wt");

  for (size_t i = 0, in = reader->GetNumberOfClasses(); i < in; ++i)
  {
    Class *clazz = reader->GetClass(i);
    if (clazz)
    {
      std::cout << inputHeaderFilename<< " <= " << clazz->GetName() << std::endl;
      //clazz->Debug();
      std::string hdr = CreateHeaderFile(clazz, "TEST_API");
      std::string src = CreateSourceFile(clazz, "TEST_API");
      fwrite(hdr.c_str(), sizeof(char), hdr.length(), headerOutput);
      fwrite(src.c_str(), sizeof(char), src.length(), sourceOutput);
      std::cout << "Header:" << std::endl << hdr << std::endl;
      std::cout << "Source:" << std::endl << src << std::endl;

    }

    clazz->Debug();
  }

  fclose(headerOutput);
  fclose(sourceOutput);


  delete reader;
  delete sourceFile;

  return 0;
}
*/

int main(int argc, char **argv)
{
  /*
  if (argc >= 2)
  {
    if (std::string(argv[1]) == std::string("--test"))
    {
      return test(argc, argv);
    }
  }
  */
  if (argc < 4)
  {
    std::cout << "Usage " << std::string(argv[0]) << " <prefix> <api> <bindir>" << std::endl;
    return -1;
  }

  struct Data
  {
    std::string inputHeaderFilename;
    std::string outputHeaderFilename;
    std::string outputSourceFilename;
    SourceFile *sourceFile;
    Reader *reader;
  };



  std::string prefix(argv[1]);
  std::string api(argv[2]);
  std::string binDir(argv[3]);

  if (prefix == std::string("NO_PREFIX"))
  {
    prefix = "";
  }
  if (api == std::string("NO_API"))
  {
    api = "";
  }


  std::string processedFilesCacheFileName = binDir + "/.csproc";
  ProcessedFilesCache processedFilesCache;
  processedFilesCache.Init(processedFilesCacheFileName);

  std::string knownClassesCacheFileName = binDir + "/.csclass";
  KnownClassesCache knownClassesCache;
  knownClassesCache.Init(knownClassesCacheFileName);

  std::string masterFileName = binDir + "/.csmoc";

  std::vector<Data> datas;

  std::wifstream masterStream(masterFileName);

  std::wstring wline;
  using convert_type = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_type, wchar_t> converter;
  while (std::getline(masterStream, wline))
  {

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    std::string line = converter.to_bytes( wline );
    Data d;
    d.sourceFile = 0;
    d.reader = 0;
    d.inputHeaderFilename = line;
    if (processedFilesCache.TouchFile(d.inputHeaderFilename))
    {
      if (!getOutputFilenames(d.inputHeaderFilename, binDir, d.outputHeaderFilename, d.outputSourceFilename, false))
      {
        continue;
      }

      d.sourceFile = new SourceFile(d.inputHeaderFilename);
      d.reader = new Reader();
      d.reader->Read(d.sourceFile);

      knownClassesCache.RemoveAllClasses(d.inputHeaderFilename);
      for (size_t i = 0, in = d.reader->GetNumberOfClasses(); i < in; ++i)
      {
        Class *cls = d.reader->GetClass(i);
        knownClassesCache.Add(d.inputHeaderFilename, cls->GetName());
      }
      processedFilesCache.UpdateFile(d.inputHeaderFilename, d.outputHeaderFilename, d.outputSourceFilename, d.reader->GetNumberOfClasses());

      datas.push_back(d);
    }
  }


  // now we know ALL classes
  for (Data &data : datas)
  {

    std::string headerSource = "#pragma once\n\n\n";
    std::string sourceSource = "";

    Reader *reader = data.reader;
    for (size_t i = 0, in = reader->GetNumberOfClasses(); i < in; ++i)
    {
      Class *clazz = reader->GetClass(i);
      if (clazz)
      {
        std::cout << data.inputHeaderFilename << " <= " << clazz->GetName() << std::endl;
        //clazz->Debug();
        headerSource += CreateHeaderFile(clazz, api);
        sourceSource += CreateSourceFile(clazz, api);

      }
    }

    size_t posSlash = data.outputHeaderFilename.find_last_of('/');
    std::string fullPath = data.outputHeaderFilename.substr(0, posSlash);
    SHCreateDirectoryEx(0, fullPath.c_str(), 0);


    //replaceSlash(path);

    FILE *header;
    fopen_s(&header, data.outputHeaderFilename.c_str(), "wt");
    fwrite(headerSource.c_str(), sizeof(char), headerSource.length(), header);
    fclose(header);

    FILE *source;
    fopen_s(&source, data.outputSourceFilename.c_str(), "wt");
    fwrite(sourceSource.c_str(), sizeof(char), sourceSource.length(), source);
    fclose(source);

  }

  if (datas.size() != 0)
  {

    // now print the master file containing all 
    //   #include <.../file.hh>
    //   #include <.../file.refl.hh>
    //   #include <.../file.refl.cc>

    // and register the modules in a static method that needs to be called from the outside manually

    std::string masterSource = "";
    std::string includeOrigin;
    std::string includeHeaders;
    std::string includeSources;
    std::string classRegistration;
    std::string classUnregistration;

    std::list<ProcessedFilesCache::CacheFile> files = processedFilesCache.GetTouchedFiles();
    for (auto it : files)
    {
      std::list<KnownClassesCache::CacheClass> classes = knownClassesCache.GetClasses(it.filename);
      if (classes.size() > 0)
      {
        std::string outputHeaderFilename, outputSourceFilename;
        if (!getOutputFilenames(it.filename, binDir, outputHeaderFilename, outputSourceFilename, true))
        {
          continue;
        }
        std::string incPrefix = prefix;
        if (incPrefix.length() != 0)
        {
          incPrefix += "/";
        }
        includeOrigin += "#include <" + incPrefix + it.filename + ">\n";
        //includeHeaders += "#include <" + prefix + "/" + outputHeaderFilename + ">\n";
        includeSources += "#include <" + incPrefix + outputSourceFilename + ">\n";

        classRegistration += "  // " + it.filename + "\n";
        classUnregistration += "  // " + it.filename + "\n";
        for (auto cls : classes)
        {
          classRegistration += "  cr->Register(" + cls.className + "Class::Get());\n";
          classUnregistration += "  cr->Unregister(" + cls.className + "Class::Get());\n";
        }
      }
    }

    masterSource += "\n";
    masterSource += "#include <stdarg.h>\n";
    masterSource += includeOrigin + "\n";
    //masterSource += includeHeaders + "\n";
    masterSource += includeSources + "\n";
    masterSource += "#include <cobalt/core/csclassregistry.hh>\n";
    masterSource += "\n\n";
    masterSource += "\n\n\n";
    masterSource += "static void register_classes ()\n";
    masterSource += "{\n";
    masterSource += "  csClassRegistry* cr = csClassRegistry::Get();\n";
    masterSource += classRegistration;
    masterSource += "}\n\n";
    masterSource += "static void unregister_classes ()\n";
    masterSource += "{\n";
    masterSource += "  csClassRegistry* cr = csClassRegistry::Get();\n";
    masterSource += classUnregistration;
    masterSource += "}\n\n";

    std::string masterFilename = binDir + "/master.refl.cc";
    FILE *source;
    fopen_s(&source, masterFilename.c_str(), "wt");
    fwrite(masterSource.c_str(), sizeof(char), masterSource.length(), source);
    fclose(source);

  }

  processedFilesCache.Output();
  knownClassesCache.Output();


  return 0;
}
