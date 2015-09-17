

#ifndef __SOURCEFILE_H__
#define __SOURCEFILE_H__

#include <vector>
#include <string>
#include <stdio.h>


class SourceFile
{
public:
  SourceFile(FILE *file); 
  ~SourceFile();

  size_t GetNumberOfLines() const;
  const std::string &GetLine(size_t line) const;

  void Debug();

private:
  std::string ReadLine(FILE *file);
  void RemoveComments();
  bool RemoveComment(std::string &line, bool inBlockComment);

  void RemoveWhiteSpaces();
  void RemoveWhiteSpaces(std::string &line);

  void RemoveEmptyLines();

  std::vector<std::string> m_lines;
};


#endif /* ! __SOURCEFILE_H__ */