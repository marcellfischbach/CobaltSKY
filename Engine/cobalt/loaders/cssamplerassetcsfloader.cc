
#include <cobalt/loaders/cssamplerassetcsfloader.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/isampler.hh>


csSamplerAssetCSFLoader::csSamplerAssetCSFLoader()
  : csBaseCSFLoader()
{

}

csSamplerAssetCSFLoader::~csSamplerAssetCSFLoader()
{

}

bool csSamplerAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  CS_UNUSED(entry);
  CS_UNUSED(locator);
  CS_UNUSED(userData);
  return entry->GetTagName() == std::string("sampler");
}


namespace
{
  csFilterMode evalFilterMode(const std::string &filterString)
  {
#define IF_FILTER(flt) if (filterString == std::string (#flt)) return eFM_##flt
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
    return eFM_MinMagMipLinear;
  }

  csTextureAddressMode evalAddressMode(const std::string &addressModeString)
  {
#define IF_ADDRESS(addr) if (addressModeString == std::string (#addr)) return eTAM_##addr
#define ELSE_IF_ADDRESS(addr) else IF_ADDRESS(addr)
    IF_ADDRESS(Repeat);
    ELSE_IF_ADDRESS(RepeatMirror);
    ELSE_IF_ADDRESS(Clamp);
    ELSE_IF_ADDRESS(ClampBorder);
    ELSE_IF_ADDRESS(MirrowOnce);
#undef IF_ADDRESS
#undef ELSE_IF_ADDRESS
    return  eTAM_Repeat;
  }

  csTextureCompareMode evalCompareMode(const std::string &compareString)
  {
#define IF_COMPARE(cmp) if (compareString == std::string (#cmp)) return eTCM_##cmp
#define ELSE_IF_COMPARE(cmp) else IF_COMPARE(cmp)
    IF_COMPARE(CompareToR);
    ELSE_IF_COMPARE(None);
#undef IF_COMPARE
#undef ELSE_IF_COMPARE
    return eTCM_None;
  }

  csTextureCompareFunc evalCompareFunc(const std::string &compareString)
  {
#define IF_COMPARE(cmp) if (compareString == std::string (#cmp)) return eTCF_##cmp
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
    return eTCF_Less;
  }


}

const csClass *csSamplerAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csSamplerWrapper::GetStaticClass();
}

csResourceWrapper *csSamplerAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  iSampler *sampler = csEng->CreateSampler();
  if (!sampler)
  {
    return nullptr;
  }
  csSamplerWrapper *samplerWrapper = new csSamplerWrapper(sampler);


  const csfEntry *filterElement = entry->GetEntry("filter");
  if (filterElement)
  {
    sampler->SetFilter(::evalFilterMode(filterElement->GetAttribute()));
  }

  const csfEntry *anisotropyElement = entry->GetEntry("anisotropy");
  if (anisotropyElement)
  {
    sampler->SetAnisotropy(anisotropyElement->GetAttributeInt());
  }

  const csfEntry *minLodElement = entry->GetEntry("minLOD");
  if (minLodElement)
  {
    sampler->SetMinLOD(minLodElement->GetAttributeInt());
  }

  const csfEntry *maxLodElement = entry->GetEntry("maxLOD");
  if (maxLodElement)
  {
    sampler->SetMaxLOD(maxLodElement->GetAttributeInt());
  }

  const csfEntry *addressUElement = entry->GetEntry("addressU");
  if (addressUElement)
  {
    sampler->SetAddressU(::evalAddressMode(addressUElement->GetAttribute()));
  }
  const csfEntry *addressVElement = entry->GetEntry("addressV");
  if (addressVElement)
  {
    sampler->SetAddressV(::evalAddressMode(addressVElement->GetAttribute()));
  }

  const csfEntry *addressWElement = entry->GetEntry("addressW");
  if (addressWElement)
  {
    sampler->SetAddressW(::evalAddressMode(addressWElement->GetAttribute()));
  }

  const csfEntry *borderColorElement = entry->GetEntry("borderColor");
  if (borderColorElement)
  {
    sampler->SetBorderColor(LoadVector4f(borderColorElement));
  }
  const csfEntry *compareModeElement = entry->GetEntry("compareMode");
  if (compareModeElement)
  {
    sampler->SetTextureCompareMode(::evalCompareMode(compareModeElement->GetAttribute()));
  }
  const csfEntry *compareFuncElement = entry->GetEntry("compareFunc");
  if (compareFuncElement)
  {
    sampler->SetTextureCompareFunc(::evalCompareFunc(compareFuncElement->GetAttribute()));
  }

  return samplerWrapper;
}

