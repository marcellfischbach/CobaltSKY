

#pragma once


#include <string>

namespace cs
{
enum eFilterMode;
enum eTextureAddressMode;
enum eTextureCompareMode;
enum eTextureCompareFunc;
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
  static std::string Filter(cs::eFilterMode filterMode);
  static std::string TextureAddressMode(eTextureAddressMode textureAddressMode);
  static std::string TextureCompareMode(eTextureCompareMode textureCompareMode);
  static std::string TextureCompareFunc(eTextureCompareFunc textureCompareFunc);

  SamplerEditorFile();


};

}

