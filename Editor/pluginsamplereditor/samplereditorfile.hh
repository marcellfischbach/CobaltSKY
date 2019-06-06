

#pragma once

#include <string>

namespace cs
{
class ResourceLocator;
struct iSampler;
}

namespace cs::file
{
class File;
class Entry;
}

namespace cs::editor::plugin::samplereditor
{

class SamplerEditorFile
{
public:

  static bool Save(const iSampler* sampler, const cs::ResourceLocator& locator);

private:
  static void Save(const iSampler* sampler, file::Entry* samplerEntry);
  static cs::file::Entry* GetEntry(cs::file::Entry* parent, const std::string& tagName);

  SamplerEditorFile();
};

}

