
#include <cobalt/core/csfwriter.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <csfile/csffile.hh>



bool cs::file::Writer::Write(const cs::file::File &file, const cs::ResourceLocator &locator, bool tight, unsigned indent)
{
  std::string fullPath = cs::VFS::Get()->GetAbsolutePath(locator, cs::VFS::DontCheckExistence);
	printf("cs::file::Writer::Write: %s\n", fullPath.c_str());
  if (fullPath.empty())
  {
    return false;
  }

  return file.Output(fullPath, tight, indent);
}

