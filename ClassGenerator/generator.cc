
#include <stdio.h>
#include <string>
#include <vector>
#include "sourcefile.hh"
#include "reader.hh"
#include "cache.hh"
#include "class.hh"
#include <ShlObj.h>


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
  result += "class " + api + " " + className + " : public vkClass\n";
  result += "{\n";
  result += "public:\n";
  result += "  " + className + "();\n";
  result += "  \n";
  result += "  static " + className + " *Get();\n";
  result += "  \n";
  result += "  virtual IObject *CreateInstance() const;\n";
  result += "  \n";
  result += "  \n";
  result += "};\n";
  result += "  \n";
  result += "  \n";
  return result;
}


static std::string CreateSourceFile(Class *clazz, const std::string &api, KnownClassesCache *classesCache)
{
  std::string className = clazz->GetName() + std::string("Class");
  std::string result = "";
  result += "\n";
  result += "\n";


  for (size_t i = 0, in = clazz->GetNumberOfProperties(); i < in; ++i)
  {
    Property prop = clazz->GetProperty(i);
    std::string propClassName = className + "_" + prop.GetPropertyName();
    result += "class " + api + " " + propClassName + " : public vkProperty\n";
    result += "{\n";
    result += "public:\n";
    result += "  " + propClassName + "() \n";
    result += "    : vkProperty (\"" + prop.GetTypeName() + "\", \"" + prop.GetPropertyName() + "\")\n";
    result += "  {\n";
    result += "  }\n";
    result += "  \n";
    result += "  virtual void SetValue(IObject *object, void *data) const\n";
    result += "  {\n";
    result += "    " + clazz->GetName() + " *d = vkQueryClass<" + clazz->GetName() + ">(object);\n";
    result += "    if (d)\n";
    result += "    {\n";
    result += "      " + prop.GetTypeName() + " &v = *reinterpret_cast<" + prop.GetTypeName() + "*>(data);\n";
    result += "      d->" + prop.GetPropertyName() + " = v;\n";
    result += "    }\n";
    result += "  }\n";
    result += "  \n";
    result += "  virtual const void *GetValue(const IObject *object) const\n";
    result += "  {\n";
    result += "    const " + clazz->GetName() + " *d = vkQueryClass<" + clazz->GetName() + ">(object);\n";
    result += "    if (!d) return 0;\n";
    result += "    return reinterpret_cast<const void*>(&d->" + prop.GetPropertyName() + ");\n";
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
  result += "  : vkClass(\"" + clazz->GetName() + "\")\n";
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
  result += "IObject *" + className + "::CreateInstance() const\n";
  result += "{\n";
  if (clazz->IsInterface())
  {
    result += "  return 0;\n";
  }
  else
  {
    result += "  return static_cast<IObject*>(new " + clazz->GetName() + "());\n";
  }
  result += "}\n";
  result += "\n";
  result += "const vkClass *" + clazz->GetName() + "::GetClass () const\n";
  result += "{\n";
  result += "  return " + className + "::Get();\n";
  result += "}\n";
  result += "\n";
  result += "const vkClass *" + clazz->GetName() + "::GetStaticClass ()\n";
  result += "{\n";
  result += "  return " + className + "::Get();\n";
  result += "}\n";
  result += "\n";
  result += "void *" + clazz->GetName() + "::QueryClass(const vkClass* clazz) \n";
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
  result += "const void *" + clazz->GetName() + "::QueryClass(const vkClass* clazz) const\n";
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


int main(int argc, char **argv)
{
  if (argc < 4)
  {
    printf("Usage %s <prefix> <api> <bindir> [infile ...]", argv[0]);
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


  std::string processedFilesCacheFileName = binDir + "/.vkproc";
  ProcessedFilesCache processedFilesCache;
  processedFilesCache.Init(processedFilesCacheFileName);

  std::string knownClassesCacheFileName = binDir + "/.vkclass";
  KnownClassesCache knownClassesCache;
  knownClassesCache.Init(knownClassesCacheFileName);

  std::vector<Data> datas;
  for (int i = 4; i < argc; ++i)
  {
    Data d;
    d.sourceFile = 0;
    d.reader = 0;
    d.inputHeaderFilename = std::string(argv[i]);

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
        printf("%s <= %s\n", data.inputHeaderFilename.c_str(), clazz->GetName().c_str());
        //clazz->Debug();
        headerSource += CreateHeaderFile(clazz, api);
        sourceSource += CreateSourceFile(clazz, api, &knownClassesCache);

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
        if (incPrefix.length () != 0)
        {
          incPrefix += "/";
        }
        includeOrigin += "#include <" + incPrefix + it.filename + ">\n";
        //includeHeaders += "#include <" + prefix + "/" + outputHeaderFilename + ">\n";
        includeSources += "#include <" + incPrefix + outputSourceFilename + ">\n";

        classRegistration += "  // " + it.filename + "\n";
        for (auto cls : classes)
        {
          classRegistration += "  cr->RegisterClass(" + cls.className + "Class::Get());\n";
        }
      }
    }

    masterSource += "\n";
    masterSource += includeOrigin + "\n";
    //masterSource += includeHeaders + "\n";
    masterSource += includeSources + "\n";
    masterSource += "#include <Valkyrie/Core/ClassRegistry.hh>\n";
    masterSource += "\n\n";
    masterSource += "\n\n\n";
    masterSource += "static void register_classes ()\n";
    masterSource += "{\n";
    masterSource += "  vkClassRegistry* cr = vkClassRegistry::Get();\n";
    masterSource += classRegistration;
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