#pragma once

#include <Valkyrie/Graphics/Mesh.hh>

namespace scenewidget
{


class ArrowMesh : public vkMesh
{
public:
  ArrowMesh();
  virtual ~ArrowMesh();

  bool Init(float tipRadius, float tipLength, float baseRadius, float baseLength, int numRads = 16);

};

}
