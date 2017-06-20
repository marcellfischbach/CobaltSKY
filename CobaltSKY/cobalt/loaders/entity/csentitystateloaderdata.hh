#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/loaders/entity/csentitystateloaderdata.refl.hh>

class csEntity;
class csEntityState;

CS_CLASS()
class csEntityStateLoaderData : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
  csEntityStateLoaderData() : csObject() { }
  virtual ~csEntityStateLoaderData() { }

public:
  csEntityState *state;
  csEntity *entity;
};

