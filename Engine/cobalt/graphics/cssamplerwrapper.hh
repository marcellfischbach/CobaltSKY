
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/isampler.hh>

#include <cobalt/graphics/cssamplerwrapper.refl.hh>

CS_CLASS()
class CSE_API csSamplerWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
public:
  csSamplerWrapper(iSampler *sampler = 0);
  virtual ~csSamplerWrapper();

  virtual void Set(iObject *sampler) override
  {
    csResourceWrapper::Set(sampler);
    m_sampler = csQueryClass<iSampler>(sampler);
  }

  CS_FORCEINLINE iSampler *Get()
  {
    return m_sampler;
  }

  CS_FORCEINLINE const iSampler *Get() const
  {
    return m_sampler;
  }

private:
  iSampler * m_sampler;
};

