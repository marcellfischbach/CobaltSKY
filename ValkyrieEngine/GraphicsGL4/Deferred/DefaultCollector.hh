#pragma once

#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Entity/Scan.hh>

class vkRenderState;
class vkDefaultCollector : public IEntityScan
{
public:
  vkDefaultCollector(vkCollection<vkRenderState*> *renderStates, vkCollection<vkLight*> *lights);

  virtual void ScanGeometry(vkRenderState *geometry);

  virtual void ScanLight(vkLight *light);


private:
  vkCollection<vkRenderState*> *m_renderStates;
  vkCollection<vkLight*> *m_lights;
};
