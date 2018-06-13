
#include <cobalt/core/csfwriter.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <csfile/csffile.hh>



bool csfWriter::Write(const csfFile &file, const csResourceLocator &locator, bool tight, unsigned indent)
{
  std::string fullPath = csVFS::Get()->GetAbsolutePath(locator, csVFS::DontCheckExistence);
  if (fullPath.empty())
  {
    return false;
  }

  return file.Output(fullPath, tight, indent);
}

