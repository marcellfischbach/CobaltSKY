
#include <valkyrie/loaders/vksamplerassetxmlloader.hh>
#include <valkyrie/graphics/isampler.hh>
#include <valkyrie/vkengine.hh>


vkSamplerAssetXMLLoader::vkSamplerAssetXMLLoader()
  : vkBaseXMLLoader()
{

}

vkSamplerAssetXMLLoader::~vkSamplerAssetXMLLoader()
{

}

bool vkSamplerAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkString(element->Value()) == vkString("sampler");
}


namespace
{
vkFilterMode evalFilterMode(const vkString &filterString)
{
#define IF_FILTER(flt) if (filterString == vkString (#flt)) return eFM_##flt
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

vkTextureAddressMode evalAddressMode(const vkString &addressModeString)
{
#define IF_ADDRESS(addr) if (addressModeString == vkString (#addr)) return eTAM_##addr
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

vkTextureCompareMode evalCompareMode(const vkString &compareString)
{
#define IF_COMPARE(cmp) if (compareString == vkString (#cmp)) return eTCM_##cmp
#define ELSE_IF_COMPARE(cmp) else IF_COMPARE(cmp)
  IF_COMPARE(CompareToR);
  ELSE_IF_COMPARE(None);
#undef IF_COMPARE
#undef ELSE_IF_COMPARE
  return eTCM_None;
}

vkTextureCompareFunc evalCompareFunc(const vkString &compareString)
{
#define IF_COMPARE(cmp) if (compareString == vkString (#cmp)) return eTCF_##cmp
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

const vkClass *vkSamplerAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return iSampler::GetStaticClass();
}

iObject *vkSamplerAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  iSampler *sampler = vkEng->CreateSampler();
  if (!sampler)
  {
    return 0;
  }


  TiXmlElement *filterElement = element->FirstChildElement("filter");
  if (filterElement)
  {
    sampler->SetFilter(::evalFilterMode(vkString(filterElement->GetText())));
  }

  TiXmlElement *anisotropyElement = element->FirstChildElement("anisotropy");
  if (anisotropyElement)
  {
    sampler->SetAnisotropy(atoi(anisotropyElement->GetText()));
  }

  TiXmlElement *minLodElement = element->FirstChildElement("minLOD");
  if (minLodElement)
  {
    sampler->SetMinLOD(atoi(minLodElement->GetText()));
  }

  TiXmlElement *maxLodElement = element->FirstChildElement("maxlLOD");
  if (maxLodElement)
  {
    sampler->SetMaxLOD(atoi(maxLodElement->GetText()));
  }

  TiXmlElement *addressUElement = element->FirstChildElement("addressU");
  if (addressUElement)
  {
    sampler->SetAddressU(::evalAddressMode(vkString(addressUElement->GetText())));
  }
  TiXmlElement *addressVElement = element->FirstChildElement("addressV");
  if (addressVElement)
  {
    sampler->SetAddressV(::evalAddressMode(vkString(addressVElement->GetText())));
  }

  TiXmlElement *addressWElement = element->FirstChildElement("addressW");
  if (addressWElement)
  {
    sampler->SetAddressW(::evalAddressMode(vkString(addressWElement->GetText())));
  }

  TiXmlElement *borderColorElement = element->FirstChildElement("borderColor");
  if (borderColorElement)
  {
    sampler->SetBorderColor(LoadVector4f(borderColorElement->GetText()));
  }
  TiXmlElement *compareModeElement = element->FirstChildElement("compareMode");
  if (compareModeElement)
  {
    sampler->SetTextureCompareMode(::evalCompareMode(vkString(compareModeElement->GetText())));
  }
  TiXmlElement *compareFuncElement = element->FirstChildElement("compareFunc");
  if (compareFuncElement)
  {
    sampler->SetTextureCompareFunc(::evalCompareFunc(vkString(compareFuncElement->GetText())));
  }

  return sampler;
}

