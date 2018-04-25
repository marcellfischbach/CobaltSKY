

#include "sourcefile.hh"
#include <stdio.h>
#include <iostream>


SourceFile::SourceFile(const std::string &filename)
{
  FILE *file;
  fopen_s(&file, filename.c_str(), "rt");
  Init(file);
}

SourceFile::SourceFile(FILE *file)
{
  Init(file);
}


void SourceFile::Init(FILE *file)
{
  m_lines.clear();
  while (!feof(file))
  {
    std::string line = ReadLine(file);
    m_lines.push_back(line);
  }

  RemoveComments();
  RemoveWhiteSpaces();
  RemoveEmptyLines();
}


SourceFile::~SourceFile()
{
  m_lines.clear();
}


size_t SourceFile::GetNumberOfLines() const
{
  return m_lines.size();
}

const std::string &SourceFile::GetLine(size_t line) const
{
  return m_lines[line];
}

std::string SourceFile::ReadLine(FILE *file)
{
  std::string line = "";
  while (!feof(file))
  {
    char ch;
    size_t numRead = fread(&ch, sizeof(char), 1, file);
    if (numRead != sizeof(char))
    {
      break;
    }
    if (ch == '\r' || ch == '\n')
    {
      break;
    }

    line += ch;
  }

  return line;
}


void SourceFile::RemoveComments()
{
  bool inBlockComment = false;
  for (size_t i = 0; i < m_lines.size(); ++i)
  {
    std::string &line = m_lines[i];
    inBlockComment = RemoveComment(line, inBlockComment);
  }
}

bool SourceFile::RemoveComment(std::string &line, bool inBlockComment)
{
  std::string newLine = "";
  bool inString = false;
  for (size_t i = 0, in = line.size(); i < in; ++i)
  {
    char ch = line[i];
    if (ch == '"')
    {
      inString = !inString;
    }

    if (!inString)
    {
      if (ch == '/')
      {
        if (i + 1 < in)
        {
          char nextCh = line[i + 1];
          if (nextCh == '/')
          {
            // this is a line comment;
            break;
          }
          else if (nextCh == '*')
          {
            // this is the start of a block comment. step over the next character an continue there (not fetching this character)
            inBlockComment = true;
            i++;
            continue;
          }
        }
      }
      else if (ch == '*')
      {
        if (i + 1 < in)
        {
          char nextCh = line[i + 1];
          if (nextCh == '/')
          {
            // this is the end of a block comment. stop over the next character an continue there (not fetching this character)
            inBlockComment = false;
            i++;
            continue;
          }
        }
      }
    }
    if (!inBlockComment)
    {
      newLine += ch;
    }
  }
  line.assign(newLine);
  return inBlockComment;
}


void SourceFile::RemoveWhiteSpaces()
{
  for (size_t i = 0; i < m_lines.size(); ++i)
  {
    std::string &line = m_lines[i];
    RemoveWhiteSpaces(line);
  }

}

void SourceFile::RemoveWhiteSpaces(std::string &line)
{
  std::string newLine = "";
  bool removeWhiteSpaces = true;
  bool inString = false;
  for (size_t i = 0, in = line.size(); i < in; ++i)
  {
    char ch = line[i];
    if (ch == '"' || ch == '\'')
    {
      inString = !inString;
    }

    if (!inString)
    {
      if (ch == ' ' || ch == '\t')
      {
        if (!removeWhiteSpaces)
        {
          newLine += ch;
          removeWhiteSpaces = true;
        }
        continue;
      }
    }
    removeWhiteSpaces = false;
    newLine += ch;
  }
  line.assign(newLine);
}

void SourceFile::RemoveEmptyLines()
{
  for (size_t i = 0; i < m_lines.size(); ++i)
  {
    std::string &line = m_lines[i];
    if (line.length() == 0)
    {
      m_lines.erase(m_lines.begin() + i);
      i--;
    }

  }

}

void SourceFile::Debug()
{
  std::cout << "SourceFile: " <<  m_lines.size() << std::endl;
  for (size_t i = 0, in = m_lines.size(); i < in; ++i)
  {
    std::cout << "(" << i+1 << ") '" << m_lines[i] << "'" << std::endl;
  }

  std::cout << "End of file" << std::endl;
}

