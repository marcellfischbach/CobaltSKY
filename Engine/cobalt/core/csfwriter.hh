
#pragma once

#include <cobalt/csexport.hh>


class csfFile;
class csResourceLocator;

class CSE_API csfWriter
{
public:
  static bool Write(const csfFile &file, const csResourceLocator &locator, bool tight = false, unsigned indent = 2);
private:
  csfWriter() { };
};