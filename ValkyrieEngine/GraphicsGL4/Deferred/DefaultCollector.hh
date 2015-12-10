#pragma once

#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Entity/Scan.hh>

class vkDefaultCollector : public IEntityScan
{
public:
  vkDefaultCollector(vkCollection<vkGeometryData*> *geometrieDatas, vkCollection<vkLight*> *lights);

  virtual void ScanGeometry(vkGeometryData *geometry);

  virtual void ScanLight(vkLight *light);


private:
  vkCollection<vkGeometryData*> *m_geometryDatas;
  vkCollection<vkLight*> *m_lights;
};
