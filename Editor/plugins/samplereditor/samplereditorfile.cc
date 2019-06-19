

#include <plugins/samplereditor/samplereditorfile.hh>

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
  cs::file::Entry* bordercolorEntry = GetEntry(samplerEntry, "bordercolor");
  cs::file::Entry* compareModeEntry = GetEntry(samplerEntry, "comparemode");
  cs::file::Entry* compareFuncEntry = GetEntry(samplerEntry, "comparefunc");

  filterEntry->RemoveAttributes()->AddAttribute(Filter(sampler->GetFilter()));
  anisotropyEntry->RemoveAttributes()->AddAttributeInt(sampler->GetAnisotropy());
  minLODEntry->RemoveAttributes()->AddAttributeInt(sampler->GetMinLOD());
  maxLODEntry->RemoveAttributes()->AddAttributeInt(sampler->GetMaxLOD());
  addressUEntry->RemoveAttributes()->AddAttribute(TextureAddressMode(sampler->GetAddressU()));
  addressVEntry->RemoveAttributes()->AddAttribute(TextureAddressMode(sampler->GetAddressV()));
  addressWEntry->RemoveAttributes()->AddAttribute(TextureAddressMode(sampler->GetAddressW()));
  bordercolorEntry->RemoveAttributes()->AddAttributeInt(0)->AddAttributeInt(0)->AddAttributeInt(0)->AddAttributeInt(0);
  compareModeEntry->RemoveAttributes()->AddAttribute(TextureCompareMode(sampler->GetTextureCompareMode()));
  compareFuncEntry->RemoveAttributes()->AddAttribute(TextureCompareFunc(sampler->GetTextureCompareFunc()));

  file.Output(absolutePath);

  return true;
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



#define CASE_STR(p, v) case p##v: return std::string(#v)

std::string SamplerEditorFile::Filter(cs::eFilterMode filter)
{
  switch (filter)
  {
    CASE_STR(eFM_, MinMagNearest);
    CASE_STR(eFM_, MinNearestMagLinear);
    CASE_STR(eFM_, MinLinearMagNearest);
    CASE_STR(eFM_, MinMagLinear);
    CASE_STR(eFM_, MinMagMipNearest);
    CASE_STR(eFM_, MinMagNearestMipLinear);
    CASE_STR(eFM_, MinNearestMagLinearMipNearest);
    CASE_STR(eFM_, MinNearestMagMipLinear);
    CASE_STR(eFM_, MinLinearMagMipNearest);
    CASE_STR(eFM_, MinLinearMagNearestMipLinear);
    CASE_STR(eFM_, MinMagLinearMipNearest);
    CASE_STR(eFM_, MinMagMipLinear);
    CASE_STR(eFM_, Anisotropic);


  }
  return "undefined";
}

std::string SamplerEditorFile::TextureAddressMode(eTextureAddressMode textureAddressMode)
{
  switch (textureAddressMode)
  {
    CASE_STR(eTAM_, Repeat);
    CASE_STR(eTAM_, RepeatMirror);
    CASE_STR(eTAM_, Clamp);
    CASE_STR(eTAM_, ClampBorder);
    CASE_STR(eTAM_, MirrowOnce);
  }
  return "undefined";
}

std::string SamplerEditorFile::TextureCompareMode(eTextureCompareMode textureCompareMode)
{
  switch (textureCompareMode)
  {
    CASE_STR(eTCM_, CompareToR);
    CASE_STR(eTCM_, None);
  }
  return "undefined";
}

std::string SamplerEditorFile::TextureCompareFunc(eTextureCompareFunc textureCompareFunc)
{
  switch (textureCompareFunc)
  {
    CASE_STR(eTCF_, LessOrEqual);
    CASE_STR(eTCF_, GreaterOrEqual);
    CASE_STR(eTCF_, Less);
    CASE_STR(eTCF_, Greater);
    CASE_STR(eTCF_, Equal);
    CASE_STR(eTCF_, NotEqual);
    CASE_STR(eTCF_, Always);
    CASE_STR(eTCF_, Never);
  }
  return "undefined";
}

}