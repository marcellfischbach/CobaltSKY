

#include <SceneWidget/Meshes.hh>

namespace scenewidget
{

ArrowMesh::ArrowMesh()
  : vkMesh ()
{

}


ArrowMesh::~ArrowMesh()
{

}

bool ArrowMesh::Init(float tipRadius, float tipLength, float baseRadius, float baseLength, int numRads)
{

  struct Vertex
  {
    vkVector3f pos;
    vkVector3f norm;
    vkVector2f txt;
  };

  int numVertices = 3 * (numRads + 1) + 2;
  Vertex *vertices = new Vertex[numVertices];


  return false;

}

}