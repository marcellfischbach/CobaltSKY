
#include "class.hh"
#include "reader.hh"
#include "sourcefile.hh"
#include "tokenizer.hh"
#include <string>
#include <map>
#include <set>


Reader::Reader()
{
}

Containers::Containers()
{
  // all std c++ containers
  m_collectionTypes.push_back(Container(std::string("std::array"), 0));
  m_collectionTypes.push_back(Container(std::string("std::vector"), 0));
  m_collectionTypes.push_back(Container(std::string("std::deque"), 0));
  m_collectionTypes.push_back(Container(std::string("std::forward_list"), 0));
  m_collectionTypes.push_back(Container(std::string("std::list"), 0));

  m_collectionTypes.push_back(Container(std::string("std::set"), 0));
  m_collectionTypes.push_back(Container(std::string("std::map"), 1));
  m_collectionTypes.push_back(Container(std::string("std::multiset"), 0));
  m_collectionTypes.push_back(Container(std::string("std::multimap"), 1));

  m_collectionTypes.push_back(Container(std::string("std::unordered_set"), 0));
  m_collectionTypes.push_back(Container(std::string("std::unordered_map"), 1));
  m_collectionTypes.push_back(Container(std::string("std::unordered_multiset"), 0));
  m_collectionTypes.push_back(Container(std::string("std::unordered_multimap"), 1));

  m_collectionTypes.push_back(Container(std::string("std::stack"), 0));
  m_collectionTypes.push_back(Container(std::string("std::queue"), 0));
  m_collectionTypes.push_back(Container(std::string("std::priority_queue"), 0));
}

bool Containers::IsCollection(const std::string &name) const
{
  for (auto &cont : m_collectionTypes)
  {
    if (cont.name == name)
    {
      return true;
    }
  }
  return false;
}

unsigned Containers::GetMasterTypeIdx(const std::string &name) const
{
  for (auto &cont : m_collectionTypes)
  {
    if (cont.name == name)
    {
      return cont.masterTypeIdx;
    }
  }
  return 0;
}

static Containers conts;

size_t readBlank(const std::string &line, size_t i)
{
  for (;i<line.length(); ++i)
  {
    if (line[i] == ' ' ||  line[i] == '\t')
    {
      continue;
    }
    break;
  }
  return i;
}

static std::set<MetaData> ReadMetaData(const std::string &line)
{
  std::set<MetaData> metaDatas;
  size_t i = 0;
  for (;i<line.length(); ++i)
  {
    if (line[i] == '(')
    {
      i++;
      break;
    }
  }
  while(i <= line.length())
  {
    std::string key = "";
    std::string value = "";

    // read blanks
    i = readBlank(line, i);
    if (!std::isalnum(line[i]) && line[i] != '_' && line[i] != '-')
    {
      break;
    }

    // read the key
    for (;i<line.length(); ++i)
    {
      char ch = line[i];

      if (std::isalnum(ch) || ch == '_' || ch == '-')
      {
        key += ch;
        continue;
      }

      break;
    }

    // read blanks
    i = readBlank(line, i);
    if (line[i] == ':' || line[i] == '=')
    {
      ++i;
      i = readBlank(line, i);
      // read the value
      for (;i<line.length(); ++i)
      {
        char ch = line[i];

        if (std::isalnum(ch) || ch == '_' || ch == '-')
        {
          value += ch;
          continue;
        }
        break;
      }
    }

    // now an optional comma can be placed
    i = readBlank(line, i);
    if (line[i] == ',')
    {
      ++i;
    }

    MetaData metaData;
    metaData.key = key;
    metaData.value = value;
    metaDatas.insert(metaData);
  }
  return metaDatas;
}


static bool isClassLine(const std::string &line)
{
  if (line.length() >= 8)
  {
    if (line.substr(0, 8) == std::string("CS_CLASS"))
    {
      if (line.length() == 8)
      {
        return true;
      }
      char c8 = line[8];
      if (c8 == '(' || c8 == ' ' || c8 == '\t' || c8 == ';')
      {
        return true;
      }
    }
  }
  return false;
}


static bool isInterfaceLine(const std::string &line)
{
  if (line.length() >= 12)
  {
    if (line.substr(0, 12) == std::string("CS_INTERFACE"))
    {
      if (line.length() == 12)
      {
        return true;
      }
      char c12 = line[12];
      if (c12 == '(' || c12 == ' ' || c12 == '\t' || c12 == ';')
      {
        return true;
      }
    }
  }
  return false;
}



static bool isPropertyLine(const std::string &line)
{
  if (line.length() >= 11)
  {
    return line.substr(0, 11) == std::string("CS_PROPERTY");
  }
  return false;
}



static bool isFunctionLine(const std::string &line)
{
  if (line.length() >= 11)
  {
    return line.substr(0, 11) == std::string("CS_FUNCTION");
  }
  return false;
}

std::string ReadClassName2(SourceFile *sourceFile, size_t lineIdx)
{
  bool searchClass = true;
  bool searchClassName = false;
  std::string lastTokenName = "";
  for (size_t n = sourceFile->GetNumberOfLines(); lineIdx < n; ++lineIdx)
  {
    Tokenizer t(sourceFile->GetLine(lineIdx));
    for (size_t i = 0, in = t.GetNumberOfTokens(); i < in; ++i)
    {
      const std::string &token = t.GetToken(i);
      if ((token == std::string("class") || token == std::string("struct")) && searchClass)
      {
        searchClass = false;
        searchClassName = true;
      }
      if (token == std::string(":") || token == std::string("{"))
      {
        return lastTokenName;
      }

      lastTokenName = token;
    }
  }
  return std::string("");
}

static Class *readClassDefinition(SourceFile *source, size_t i, bool interf)
{
  std::string line = source->GetLine(i);
  std::set<MetaData> metaData = ReadMetaData(line);
  ++i;

  bool readName = false;
  std::string lastToken = "";
  Class *clazz = nullptr;
  bool readSuperName = false;
  
  for (size_t in = source->GetNumberOfLines(); i < in; ++i)
  {
    Tokenizer tokenizer(source->GetLine(i));
    for (size_t j = 0, jn = tokenizer.GetNumberOfTokens(); j < jn; ++j)
    {
      std::string token = tokenizer.GetToken(j);

      if (token == "{")
      {
        if (readName)
        {
          clazz = new Class();
          clazz->SetInterface(interf);
          clazz->SetName(lastToken);
        }
        return clazz;
      }
      else if (token == "class" || token == "struct")
      {
        readName = true;
      }
      else if (token == ":")
      {
        if (!readName)
        {
          return nullptr;
        }
        clazz = new Class();
        clazz->SetInterface(interf);
        clazz->SetName(lastToken);
        clazz->SetMeta(metaData);
        readName = false;
        readSuperName = false;
      }
      else if (token == "public" || token == "protected" || token == "private" || token == "virtual" || token == "," || token == "(" || token == ")")
      {
        continue;
      }
      else if (token == "CS_SUPER")
      {
        readSuperName = true;
      }
      else if (readSuperName && clazz)
      {
        clazz->AddSuperClass(token);
        readSuperName = false;
      }





      lastToken = token;
    }
  }


  return nullptr;
}



std::string leftTrim(const std::string &str)
{
  bool trim = true;
  std::string result = "";
  for (size_t i = 0, in = str.length(); i < in; ++i)
  {
    char ch = str[i];
    if (ch == ' ' || ch == '\t')
    {
      if (trim)
      {
        continue;
      }
    }
    else
    {
      trim = false;
    }

    result += ch;
  }

  return result;
}


void ReadPropertyMetaData(SourceFile *sourceFile, size_t lineIdx, std::map<std::string, std::string> &data)
{
  Tokenizer t(sourceFile->GetLine(lineIdx));
  for (size_t i = 0, in = t.GetNumberOfTokens(); i < in; ++i)
  {
    const std::string &token = t.GetToken(i);
    if (token == "," || token == ")")
    {
      std::string metaValue = "", metaKey = "name";
      if (i >= 1)
      {
        metaValue = t.GetToken(i - 1);
      }
      if (i >= 3 && t.GetToken(i-2) == "=")
      {
        metaKey = t.GetToken(i - 3);
      }
      else
      {
        metaKey = metaValue;
        metaValue = "";
      }

      if (!metaKey.empty())
      {
        data[metaKey] = metaValue;
      }
    }
  }
}

namespace
{
std::string FirstLetterUpperCase(const std::string name)
{
  std::string result = name;
  if (result.length() > 0)
  {
    char ch = result[0];
    if (ch >= 'a' && ch <= 'z')
    {
      ch += 'A' - 'a';
    }
    result[0] = ch;
  }
  return result;
}
std::string ConvertPropertyName(const std::string propertyName)
{
  size_t idx = 0;
  std::string result = "";
  if (propertyName.length() > 2)
  {
    if (propertyName[0] == 'm' && propertyName[1] == '_')
    {
      idx = 2;
    }
  }
  if (propertyName.length() > 1)
  {
    if (propertyName[0] == '_')
    {
      idx = 1;
    }
  }
  return FirstLetterUpperCase(propertyName.substr(idx));
}
}



Property ReadProperty(SourceFile *sourceFile, size_t lineIdx)
{
  std::map<std::string, std::string> meta;
  ReadPropertyMetaData(sourceFile, lineIdx - 1, meta);
  
  enum States
  {
    ConstOrTypename,
    ContainerStart,
    ContainerConstOrTypename,
    ContainerTypeSpecificationOrEnd,
    SpecificationOrName,
    Done,
  };

  States state = ConstOrTypename;

  bool isContainerConst = false;
  std::string containerTypeName = "";
  TypeSpecifiction containerTypeSpecification = eTS_Value;

  bool isConst = false;
  std::string typeName = "";
  TypeSpecifiction typeSpecification = eTS_Value;
  std::string paramName;

  bool isCollection = false;
  unsigned masterIdx = 0;

  

  for (size_t n = sourceFile->GetNumberOfLines(); lineIdx < n; ++lineIdx)
  {
    Tokenizer t(sourceFile->GetLine(lineIdx));
    for (size_t i = 0, in = t.GetNumberOfTokens(); i < in; ++i)
    {
      const std::string &token = t.GetToken(i);
      size_t semiIdx = token.find(';');
      if (semiIdx != std::string::npos)
      {
        return Property(isContainerConst, containerTypeName, containerTypeSpecification, isConst, typeName, typeSpecification, paramName, meta);
      }

      switch (state)
      {
      case ConstOrTypename:
        if (token == std::string("const"))
        {
          isConst = true;
        }
        else
        {
          typeName = token;
          if (conts.IsCollection(typeName))
          {
            isCollection = true;
            masterIdx = conts.GetMasterTypeIdx(typeName);
            state = ContainerStart;
          }
          else
          {
            state = SpecificationOrName;
          }
        }
        break;
      case ContainerStart:
        if (token == std::string("<"))
        {
          state = ContainerConstOrTypename;
        }
        break;

      case ContainerConstOrTypename:
        if (token == std::string(","))
        {
          break;
        }

        if (masterIdx == 0)
        {
          if (token == std::string("const"))
          {
            isContainerConst = true;
          }
          else
          {
            containerTypeName = token;
            state = ContainerTypeSpecificationOrEnd;
          }
        }
        else
        {
          masterIdx--;
        }
        break;

      case ContainerTypeSpecificationOrEnd:
        if (token == std::string("&"))
        {
          containerTypeSpecification = eTS_Reference;
        }
        else if (token == std::string("*"))
        {
          containerTypeSpecification = eTS_Pointer;
        }
        else if (token == std::string("**"))
        {
          containerTypeSpecification = eTS_PointerToPointer;
        }
        else if (token == ">")
        {
          state = SpecificationOrName;
        }
        break;

      case SpecificationOrName:
        if (token == std::string("&"))
        {
          typeSpecification = eTS_Reference;
        }
        else if (token == std::string("*"))
        {
          typeSpecification = eTS_Pointer;
        }
        else if (token == std::string("**"))
        {
          typeSpecification = eTS_PointerToPointer;
        }
        else
        {
          paramName = ConvertPropertyName(token);
          state = Done;
        }
        break;

      case Done:
        break;

      }
    }
  }
  return Property(false, "", eTS_Value, false, "", eTS_Value, "", meta);
}

Function ReadFunction(SourceFile *sourceFile, size_t lineIdx)
{
  enum States
  {
    VirtualOrConstOrTypename,
    SpecifierOrFunctionName,
    LeftParen,

    ConstOrArgTypeNameOrRightParen,
    SpecifierOrArgName,
    CommaOrRightParen,
    ConstOrOverride,
  };
  States state = VirtualOrConstOrTypename;
  Function function;

  bool returnTypeConst = false;
  std::string typeName = "";
  bool isVirtual = false;
  TypeSpecifiction returnTypeSpecification = eTS_Value;
  std::string functionName = "";

  bool argConst = false;
  std::string argType;
  TypeSpecifiction argTypeSpecification = eTS_Value;
  std::string argName;

  for (size_t n = sourceFile->GetNumberOfLines(); lineIdx < n; ++lineIdx)
  {
    Tokenizer t(sourceFile->GetLine(lineIdx));
    for (size_t i = 0, in = t.GetNumberOfTokens(); i < in; ++i)
    {
      const std::string &token = t.GetToken(i);
      size_t semiIdx = token.find(';');
      if (semiIdx != std::string::npos)
      {
        return function;
      }


      switch (state)
      {
      case VirtualOrConstOrTypename:
        if (token == std::string("virtual"))
        {
          function.SetVirtual(true);
        }
        else if (token == std::string("const"))
        {
          returnTypeConst = true;
        }
        else
        {
          typeName = token;
          state = SpecifierOrFunctionName;
        }
        break;

      case SpecifierOrFunctionName:
        if (token == std::string("&"))
        {
          returnTypeSpecification = eTS_Reference;
        }
        else if (token == std::string("*"))
        {
          returnTypeSpecification = eTS_Pointer;
        }
        else if (token == std::string("**"))
        {
          returnTypeSpecification = eTS_PointerToPointer;
        }
        else
        {
          functionName = token;
          state = LeftParen;
        }
        break;

      case LeftParen:
        if (token == "(")
        {
          function.SetName(functionName);
          function.SetReturnType(typeName, returnTypeConst, returnTypeSpecification);

          state = ConstOrArgTypeNameOrRightParen;
        }
        else
        {
          return Function();
        }
        break;

      case ConstOrArgTypeNameOrRightParen:
        if (token == "const")
        {
          argConst = true;
        }
        else if (token == ")")
        {
          if (!argName.empty() && !argType.empty())
          {
            function.AddParameter(argName, argType, argConst, argTypeSpecification);
          }
          state = ConstOrOverride;
        }
        else
        {
          argType = token;
          state = SpecifierOrArgName;
        }
        break;

      case SpecifierOrArgName:
        if (token == std::string("&"))
        {
          argTypeSpecification = eTS_Reference;
        }
        else if (token == std::string("*"))
        {
          argTypeSpecification = eTS_Pointer;
        }
        else if (token == std::string("**"))
        {
          argTypeSpecification = eTS_PointerToPointer;
        }
        else
        {
          argName = token;
          state = CommaOrRightParen;
        }
        break;

      case CommaOrRightParen:
        if (!argName.empty() && !argType.empty())
        {
          function.AddParameter(argName, argType, argConst, argTypeSpecification);
        }
        argName = "";
        argType = "";
        argConst = false;
        argTypeSpecification = eTS_Value;

        if (token == ",")
        {
          state = ConstOrArgTypeNameOrRightParen;
        }
        else if (token == std::string(")"))
        {
          state = ConstOrOverride;
        }
        break;
      case ConstOrOverride:
        if (token == std::string("const"))
        {
          function.SetConst(true);
        }
        else if (token == std::string("override"))
        {
          function.SetOverride(true);
        }
        break;
      }

    }
  }
  return function;
}


void Reader::Read(SourceFile *source)
{
  Class *cl = 0;
  for (size_t i = 0, in = source->GetNumberOfLines(); i < in; ++i)
  {
    const std::string &line = source->GetLine(i);
    if (isClassLine(line))
    {
      cl = readClassDefinition(source, i, false);
      if (cl)
      {
        m_classes.push_back(cl);
      }
    }
    else if (isInterfaceLine(line))
    {
      cl = readClassDefinition(source, i, true);
      if (cl)
      {
        m_classes.push_back(cl);
      }
    }
    else if (isFunctionLine(line) && cl)
    {
      i++;
      if (i >= in)
      {
        return;
      }
      Function function = ReadFunction(source, i);
      if (function.IsValid())
      {
        cl->AddFunction(function);
      }
    }
    else if (isPropertyLine(line) && cl)
    {
      i++;
      if (i >= in)
      {
        return;
      }
      Property property = ReadProperty(source, i);
      if (property.IsValid())
      {
        cl->AddProperty(property);
      }
    }
  }
}

size_t Reader::GetNumberOfClasses() const
{
  return m_classes.size();
}

Class *Reader::GetClass(size_t idx)
{
  return m_classes[idx];
}
