
#include <cobalt/loaders/cssamplerassetcsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/isampler.hh>


cs::SamplerAssetCSFLoader::SamplerAssetCSFLoader()
  : cs::BaseCSFLoader()
{

}

cs::SamplerAssetCSFLoader::~SamplerAssetCSFLoader()
{

}

bool cs::SamplerAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return entry->GetTagName() == std::string("sampler");
}


namespace
{
  cs::eFilterMode evalFilterMode(const std::string &filterString)
  {
#define IF_FILTER(flt) if (filterString == std::string (#flt)) return cs::eFM_##flt
#define ELSE_IF_FILTER(flt) else IF_FILTER(flt)

    IF_FILTER(MinMagNearest);
    ELSE_IF_FILTER(MinNearestMagLinear);
    ELSE_IF_FILTER(MinLinearMagNearest);
    ELSE_IF_FILTER(MinMagLinear);
    ELSE_IF_FILTER(MinMagMipNearest);
    ELSE_IF_FILTER(MinMagNearestMipLinear);
    ELSE_IF_FILTER(MinNearestMagLinearMipNearest);
    ELSE_IF_FILTER(MinNearestMagMipLinear);
    ELSE_IF_FILTER(MinLinearMagMipNearest);
    ELSE_IF_FILTER(MinLinearMagNearestMipLinear);
    ELSE_IF_FILTER(MinMagLinearMipNearest);
    ELSE_IF_FILTER(MinMagMipLinear);
    ELSE_IF_FILTER(Anisotropic);
#undef IF_FILTER
#undef ELSE_IF_FILTER
    return cs::eFM_MinMagMipLinear;
  }

  cs::eTextureAddressMode evalAddressMode(const std::string &addressModeString)
  {
#define IF_ADDRESS(addr) if (addressModeString == std::string (#addr)) return cs::eTAM_##addr
#define ELSE_IF_ADDRESS(addr) else IF_ADDRESS(addr)
    IF_ADDRESS(Repeat);
    ELSE_IF_ADDRESS(RepeatMirror);
    ELSE_IF_ADDRESS(Clamp);
    ELSE_IF_ADDRESS(ClampBorder);
    ELSE_IF_ADDRESS(MirrowOnce);
#undef IF_ADDRESS
#undef ELSE_IF_ADDRESS
    return  cs::eTAM_Repeat;
  }

  cs::eTextureCompareMode evalCompareMode(const std::string &compareString)
  {
#define IF_COMPARE(cmp) if (compareString == std::string (#cmp)) return cs::eTCM_##cmp
#define ELSE_IF_COMPARE(cmp) else IF_COMPARE(cmp)
    IF_COMPARE(CompareToR);
    ELSE_IF_COMPARE(None);
#undef IF_COMPARE
#undef ELSE_IF_COMPARE
    return cs::eTCM_None;
  }

  cs::eTextureCompareFunc evalCompareFunc(const std::string &compareString)
  {
#define IF_COMPARE(cmp) if (compareString == std::string (#cmp)) return cs::eTCF_##cmp
#define ELSE_IF_COMPARE(cmp) else IF_COMPARE(cmp)
    IF_COMPARE(LessOrEqual);
    ELSE_IF_COMPARE(GreaterOrEqual);
    ELSE_IF_COMPARE(Less);
    ELSE_IF_COMPARE(Greater);
    ELSE_IF_COMPARE(Equal);
    ELSE_IF_COMPARE(NotEqual);
    ELSE_IF_COMPARE(Always);
    ELSE_IF_COMPARE(Never);
#undef IF_COMPARE
#undef ELSE_IF_COMPARE
    return cs::eTCF_Less;
  }


}

const cs::Class *cs::SamplerAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::SamplerWrapper::GetStaticClass();
}

cs::ResourceWrapper *cs::SamplerAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::iSampler *sampler = csEng->CreateSampler();
  if (!sampler)
  {
    return nullptr;
  }
  cs::SamplerWrapper *samplerWrapper = new cs::SamplerWrapper(sampler);


  const cs::file::Entry *filterElement = entry->GetEntry("filter");
  if (filterElement)
  {
    sampler->SetFilter(::evalFilterMode(filterElement->GetAttribute()));
  }

  const cs::file::Entry *anisotropyElement = entry->GetEntry("anisotropy");
  if (anisotropyElement)
  {
    sampler->SetAnisotropy(anisotropyElement->GetAttributeInt());
  }

  const cs::file::Entry *minLodElement = entry->GetEntry("minLOD");
  if (minLodElement)
  {
    sampler->SetMinLOD(minLodElement->GetAttributeInt());
  }

  const cs::file::Entry *maxLodElement = entry->GetEntry("maxLOD");
  if (maxLodElement)
  {
    sampler->SetMaxLOD(maxLodElement->GetAttributeInt());
  }

  const cs::file::Entry *addressUElement = entry->GetEntry("addressU");
  if (addressUElement)
  {
    sampler->SetAddressU(::evalAddressMode(addressUElement->GetAttribute()));
  }
  const cs::file::Entry *addressVElement = entry->GetEntry("addressV");
  if (addressVElement)
  {
    sampler->SetAddressV(::evalAddressMode(addressVElement->GetAttribute()));
  }

  const cs::file::Entry *addressWElement = entry->GetEntry("addressW");
  if (addressWElement)
  {
    sampler->SetAddressW(::evalAddressMode(addressWElement->GetAttribute()));
  }

  const cs::file::Entry *borderColorElement = entry->GetEntry("borderColor");
  if (borderColorElement)
  {
    sampler->SetBorderColor(LoadVector4f(borderColorElement));
  }
  const cs::file::Entry *compareModeElement = entry->GetEntry("compareMode");
  if (compareModeElement)
  {
    sampler->SetTextureCompareMode(::evalCompareMode(compareModeElement->GetAttribute()));
  }
  const cs::file::Entry *compareFuncElement = entry->GetEntry("compareFunc");
  if (compareFuncElement)
  {
    sampler->SetTextureCompareFunc(::evalCompareFunc(compareFuncElement->GetAttribute()));
  }

  return samplerWrapper;
}

