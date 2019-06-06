

#include <pluginsamplereditor/samplereditorfile.hh>

#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/isampler.hh>

#include <csfile/csffile.hh>


namespace cs::editor::plugin::samplereditor
{

SamplerEditorFile::SamplerEditorFile()
{

}

bool SamplerEditorFile::Save(const iSampler* sampler, const cs::ResourceLocator& locator)
{

  std::string absolutePath = cs::VFS::Get()->GetAbsolutePath(locator, cs::VFS::CheckExistence);
  cs::file::File file;
  if (!absolutePath.empty())
  {

    file.Parse(absolutePath);
  }

  cs::file::Entry* root = file.GetRoot();
  cs::file::Entry* assetEntry = GetEntry(root, "asset");
  cs::file::Entry* dataEntry = GetEntry(assetEntry, "data");
  cs::file::Entry* samplerEntry = GetEntry(dataEntry, "sampler");


  cs::file::Entry* filterEntry = GetEntry(samplerEntry, "filter");
  cs::file::Entry* anisotropyEntry = GetEntry(samplerEntry, "anisotropy");
  cs::file::Entry* minLODEntry = GetEntry(samplerEntry, "minlod");
  cs::file::Entry* maxLODEntry = GetEntry(samplerEntry, "maxlod");
  cs::file::Entry* addressUEntry = GetEntry(samplerEntry, "addressu");
  cs::file::Entry* addressVEntry = GetEntry(samplerEntry, "addressv");
  cs::file::Entry* addressWEntry = GetEntry(samplerEntry, "addressw");
  cs::file::Entry* addressWEntry = GetEntry(samplerEntry, "addressw");
  cs::file::Entry* bordercolorEntry = GetEntry(samplerEntry, "bordercolor");
  cs::file::Entry* compareModeEntry = GetEntry(samplerEntry, "comparemode");
  cs::file::Entry* compareFuncEntry = GetEntry(samplerEntry, "comparefunc");


}

cs::file::Entry* SamplerEditorFile::GetEntry(cs::file::Entry* parent, const std::string& tagName)
{
  cs::file::Entry* entry = parent->GetEntry(tagName);
  if (!entry)
  {
    entry = parent->GetFile()->CreateEntry(tagName);
    parent->AddChild(entry);
  }
  return entry;
}

}