
#include "class.hh"
#include "reader.hh"
#include "sourcefile.hh"
#include "tokenizer.hh"
#include <string>


Reader::Reader()
{

}

static bool isClassLine(const std::string &line)
{
  if (line.length() >= 8)
  {
    if (line.substr(0, 8) == std::string("VK_CLASS"))
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
    if (line.substr(0, 12) == std::string("VK_INTERFACE"))
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
    return line.substr(0, 11) == std::string("VK_PROPERTY");
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
  bool readName = false;
  std::string lastToken = "";
  Class *clazz = 0;
  bool readSuperNames = false;
  
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
          return 0;
        }
        clazz = new Class();
        clazz->SetInterface(interf);
        clazz->SetName(lastToken);
        readName = false;
        readSuperNames = true;
      }
      else if (token == "public" || token == "protected" || token == "private" || token == "virtual" || token == ",")
      {
        continue;
      }
      else if (readSuperNames && clazz)
      {
        clazz->AddSuperClass(token);
      }





      lastToken = token;
    }
  }


  return 0;
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
Property ReadProperty(SourceFile *sourceFile, size_t lineIdx)
{
  std::string typeName = "";
  std::string paramName;
  std::string lastTokenName = "";
  for (size_t n = sourceFile->GetNumberOfLines(); lineIdx < n; ++lineIdx)
  {
    Tokenizer t(sourceFile->GetLine(lineIdx));
    for (size_t i = 0, in = t.GetNumberOfTokens(); i < in; ++i)
    {
      const std::string &token = t.GetToken(i);
      size_t semiIdx = token.find(';');
      if (semiIdx == std::string::npos)
      {
        typeName += " " + paramName;
        paramName = token;
      }
      else
      {
        if (semiIdx != 0)
        {
          typeName += " " + paramName;
          paramName = token.substr(0, semiIdx);
        }

        if (typeName.length() > 0)
        {
          typeName = leftTrim(typeName);
        }
        return Property(typeName, paramName);
      }
    }
  }
  return Property("", "");
}


void Reader::Read(SourceFile *source)
{
  Class *cl = 0;
  for (size_t i = 0, in = source->GetNumberOfLines(); i < in; ++i)
  {
    const std::string &line = source->GetLine(i);
    if (isClassLine(line))
    {
      i++;
      if (i >= in)
      {
        return;
      }

      cl = readClassDefinition(source, i, false);
      if (cl)
      {
        m_classes.push_back(cl);
      }
    }
    else if (isInterfaceLine(line))
    {
      i++;
      if (i >= in)
      {
        return;
      }

      cl = readClassDefinition(source, i, true);
      if (cl)
      {
        m_classes.push_back(cl);
      }
    }
    else if (isPropertyLine(line) && cl)
    {
      i++;
      if (i >= in)
      {
        return ;
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
