
#pragma once

#include <cobalt/csexport.hh>


class csfFile;

namespace cs
{
class ResourceLocator;

namespace file
{



class CSE_API Writer
{
public:
  static bool Write(const csfFile& file, const cs::ResourceLocator& locator, bool tight = false, unsigned indent = 2);
private:
  Writer() { };
};

}
}
