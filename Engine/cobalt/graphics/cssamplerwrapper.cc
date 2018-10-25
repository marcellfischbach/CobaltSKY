
#include <cobalt/graphics/cssamplerwrapper.hh>


csSamplerWrapper::csSamplerWrapper(iSampler *sampler)
  : csResourceWrapper(sampler)
  , m_sampler(sampler)
{
}
