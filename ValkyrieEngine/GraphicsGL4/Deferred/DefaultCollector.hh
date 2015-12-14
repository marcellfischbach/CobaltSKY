#pragma once

#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Entity/Scan.hh>

class vkDefaultCollector : public IEntityScan
{
public:
  vkDefaultCollector(vkCollection<vkGeometryMesh*> *geometrieMeshes, vkCollection<vkLight*> *lights);

  virtual void ScanGeometry(vkGeometryMesh *geometry);

  virtual void ScanLight(vkLight *light);


private:
  vkCollection<vkGeometryMesh*> *m_geometryMeshes;
  vkCollection<vkLight*> *m_lights;
};
