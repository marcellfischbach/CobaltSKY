
#pragma once

#include <cobalt/csexport.hh>



namespace cs
{
class ResourceLocator;

namespace file
{

class File;


class CSE_API Writer
{
public:
  static bool Write(const cs::file::File& file, const cs::ResourceLocator& locator, bool tight = false, unsigned indent = 2);
private:
  Writer() { };
};

}
}
