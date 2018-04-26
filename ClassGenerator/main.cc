

#include <stdio.h>
#include "sourcefile.hh"
#include "class.hh"
#include "reader.hh"
#include <ShlObj.h>
#include <iostream>

std::string prefix;

std::string CreateHeaderFile(Class *clazz, const std::string &api)
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
  result += "  virtual iObject   *CreateInstance() const;\n";
  result += "  \n";
  result += "  \n";
  result += "};\n";
  result += "  \n";
  result += "  \n";
  return result;
}


std::string CreateSourceFile(Class *clazz, const std::string &api)
{
  std::string className = clazz->GetName() + std::string("Class");
  std::string result = "";
  result += "\n";
  result += "\n";


  for (size_t i = 0, in = clazz->GetNumberOfProperties(); i < in; ++i)
  {
    Property prop = clazz->GetProperty(i);
    std::string propClassName = className + "_NARF_" + prop.GetPropertyName();
    result += "class " + api + " " + propClassName + " : public csProperty\n";
    result += "{\n";
    result += "public:\n";
    result += "  " + propClassName + "() \n";
    result += "    : csProperty (\"" + prop.GetTypeName() + "\", \"" + prop.GetPropertyName() + "\")\n";
    result += "  {\n";
    result += "  }\n";
    result += "  \n";
    result += "  virtual void SetValue(iObject *object, void *data) const\n";
    result += "  {\n";
    result += "    " + clazz->GetName() + " *d = csQueryClass<" + clazz->GetName() + ">(object);\n";
    result += "    if (d)\n";
    result += "    {\n";
    result += "      " + prop.GetTypeName() + " &v = *reinterpret_cast<" + prop.GetTypeName() + "*>(data);\n";
    result += "      d->Set" + prop.GetPropertyName() + "(v);\n";
    result += "    }\n";
    result += "  }\n";
    result += "  \n";
    result += "  virtual const void *GetValue(const iObject *object) const\n";
    result += "  {\n";
    result += "    const " + clazz->GetName() + " *d = csQueryClass<" + clazz->GetName() + ">(object);\n";
    result += "    if (!d) return 0;\n";
    result += "    return reinterpret_cast<const void*>(&d->Get" + prop.GetPropertyName() + "());\n";
    result += "  }\n";
    result += "  \n";
    result += "};\n";
    result += "\n";
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

  for (size_t i = 0, in = clazz->GetNumberOfProperties(); i < in; ++i)
  {
    Property prop = clazz->GetProperty(i);
    std::string propClassName = className + "_" + prop.GetPropertyName();
    result += "  AddProperty(new " + propClassName + "());\n";
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
      result += "  super = " + clazz->GetSuperClass(s) + "::QueryClass(clazz);\n";
      result += "  if (super)\n";
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
      result += "  if (super)\n";
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

void replaceSlash(std::string &path)
{
  for (size_t i = 0, in = path.length(); i < in; ++i)
  {
    if (path[i] == '\\')
    {
      path[i] = '/';
    }
  }
}

bool getFiletime(const std::string &fileName, LPFILETIME fileTime)
{

  bool result = false;

  HANDLE handle = CreateFile(fileName.c_str(),
                             GENERIC_READ,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0);
  if (handle)
  {
    if (GetFileTime(handle, 0, 0, fileTime))
    {
      result = true;
    }
    CloseHandle(handle);
  }


  return result;
}


bool setFiletime(const std::string &fileName, LPFILETIME fileTime)
{
  bool result = false;
  HANDLE handle = CreateFile(fileName.c_str(),
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0);
  if (handle)
  {
    if (SetFileTime(handle, 0, fileTime, fileTime))
    {
      result = true;
    }
    CloseHandle(handle);
  }

  return result;
}

bool equals(FILETIME &ftA, FILETIME &ftB)
{
  return ftA.dwHighDateTime == ftB.dwHighDateTime &&
    ftA.dwLowDateTime == ftB.dwLowDateTime;
}

int perform (const char *in_filename, const char *in_prefix, const char* api, const char *in_outputDirectory)
{
  std::string inputHeaderFileName = std::string(in_filename);
  size_t dotPos = inputHeaderFileName.find_last_of('.');
  if (dotPos == std::string::npos)
  {
    return 0;
  }
  std::string ext = inputHeaderFileName.substr(dotPos);
  if (ext != ".h" && ext != ".hh" && ext != ".hpp")
  {
    return 0;
  }

  std::string filename = std::string(in_filename);
  replaceSlash(filename);
  size_t posDot = filename.find_last_of('.');


  std::string name = filename.substr(0, posDot);


  std::string path = std::string(in_outputDirectory) + '/' + name;

  size_t posSlash = path.find_last_of('/');
  std::string fullPath = path.substr(0, posSlash);



  // now those two vars contains the final resulting files
  std::string headerName = path + ".refl.hh";
  std::string sourceName = path + ".refl.cc";


  FILETIME ftSource, ftDestHH, ftDestCC;
  bool gftSource = getFiletime(in_filename, &ftSource);
  bool gftDestHH = getFiletime(headerName, &ftDestHH);
  bool gftDestCC = getFiletime(headerName, &ftDestCC);

  if (gftSource && gftDestHH && gftDestCC)
  {
    if (equals(ftSource, ftDestCC) && equals(ftSource, ftDestHH))
    {
      // all times are equals .. so no futher work is needed
      return 1;
    }
  }

  FILE *file;
  fopen_s(&file, in_filename, "rt");
  if (!file)
  {
    std::cout << "File not found. [" << std::string(in_filename) << "]" << std::endl;
    return 0;
  }




  prefix = std::string(in_prefix);

  SourceFile sourceFile(file);

  Reader reader;
  reader.Read(&sourceFile);

  if (reader.GetNumberOfClasses() == 0)
  {
    return 0;
  }
  std::cout << "GenRefl for " << std::string(in_filename) << std::endl;

  std::string headerSource = "";
  std::string sourceSource = "";

  headerSource += "\n";
  headerSource += "#pragma once\n";
  headerSource += "\n";
  headerSource += "#include <valkyrie/core/csclass.hh>\n";
  headerSource += "\n";

  sourceSource += "\n";
  sourceSource += "#include <" + prefix + "/" + in_filename + ">\n";
  sourceSource += "\n";



  for (size_t i = 0, in = reader.GetNumberOfClasses(); i < in; ++i)
  {
    Class *clazz = reader.GetClass(i);
    if (clazz)
    {
      //clazz->Debug();
      headerSource += CreateHeaderFile(clazz, api);
      sourceSource += CreateSourceFile(clazz, api);
    }
  }


  SHCreateDirectoryEx(0, fullPath.c_str(), 0);


  replaceSlash(path);

  FILE *header;
  fopen_s(&header, headerName.c_str(), "wt");
  fwrite(headerSource.c_str(), sizeof(char), headerSource.length(), header);
  fclose(header);

  FILE *source;
  fopen_s(&source, sourceName.c_str(), "wt");
  fwrite(sourceSource.c_str(), sizeof(char), sourceSource.length(), source);
  fclose(source);

  setFiletime(headerName, &ftSource);
  setFiletime(sourceName, &ftSource);

  return 1;
}


int main2(int argc, char **argv)
{
  if (argc < 5)
  {
    std::cout << "Usage " << std::string(argv[0]) << " <mode> <prefix> <api> <outputpath> <input> [...<input>]" << std::endl;
    for (int i = 0; i < argc; ++i)
    {
      std::cout << "  arg: '" << std::string(argv[i]) << "'" << std::endl;
    }
    return -1;
  }
  if (std::string(argv[1]) == std::string("--list"))
  {
    for (int i = 5; i < argc; ++i)
    {
      perform(argv[i], argv[2], argv[3], argv[4]);
    }
    return 0;
  }
  else if (std::string(argv[1]) == std::string("--file"))
  {
    return perform(argv[5], argv[2], argv[3], argv[4]);
  }

  return -1;
}
