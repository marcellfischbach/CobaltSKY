

#include <preview/previewsceneview.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/entity/vklightstate.hh>
#include <valkyrie/entity/vkstaticmeshstate.hh>
#include <valkyrie/graphics/vkdirectionallight.hh>
#include <valkyrie/graphics/vkmaterialinstance.hh>
#include <valkyrie/graphics/vkmesh.hh>
#include <valkyrie/graphics/vksubmesh.hh>

PreviewSceneView::PreviewSceneView(QWidget *parent)
  : SceneView(parent)
{
  vkEntityScene *scene = new vkEntityScene();
  SetScene(scene);

  //
  // create the light within the scene
  m_light = new vkDirectionalLight();
  m_lightState = new vkLightState();
  m_lightState->SetLight(m_light);
  m_lightEntity = new vkEntity();
  m_lightEntity->SetRootState(m_lightState);
  m_lightEntity->AddState(m_lightState);
  m_lightEntity->FinishTransformation();
  scene->AddEntity(m_lightEntity);
}

PreviewSceneView::~PreviewSceneView()
{

}



vkEntity *PreviewSceneView::CreateSphere(float radius, unsigned numR, unsigned numV, vkMaterialInstance *materialInstance)
{
  unsigned numVertices = (numR+1) * (numV+1);
  unsigned numIndices = numR * numV * 3 * 2;

  struct Vertex
  {
    vkVector3f co;
    vkVector3f no;
    vkVector3f ta;
    vkVector3f bt;
    vkVector2f tx;
  };
  
  Vertex *vertices = new Vertex[numVertices];

  for (unsigned v = 0, i = 0; v <= numV; ++v)
  {
    float factV = (float)v / (float)numV;
    float angleV = factV * 3.141569f;
    for (unsigned r = 0; r <= numR; ++r, ++i)
    {
      float factR = (float)r / (float)numR;
      float angleR = factR * 2.0f * 3.141569f;

      vertices[i].no = vkVector3f(
        cos(angleR) * sin(angleV),
        sin(angleR) * sin(angleV),
        cos(angleV)
      );
      vertices[i].co = vertices[i].no * radius;
      vertices[i].ta = vkVector3f(
        -sin(angleR),
        cos(angleR),
        0.0f
      );
      vertices[i].bt = vkVector3f::Cross(vertices[i].no, vertices[i].ta);
      vertices[i].tx = vkVector2f(factR, factV);
    }
  }

  unsigned short *indices = new unsigned short[numIndices];
  unsigned short *iptr = indices;
  for (unsigned v = 0; v < numV; ++v)
  {
    unsigned v0 = v * numR;
    unsigned v1 = v0 + numR;

    for (unsigned r = 0; r < numR; ++r)
    {
      unsigned v00 = v0 + r;
      unsigned v01 = v00 + 1;
      unsigned v10 = v1 + r;
      unsigned v11 = v10 + 1;

      *iptr++ = v00;
      *iptr++ = v01;
      *iptr++ = v11;
      *iptr++ = v00;
      *iptr++ = v11;
      *iptr++ = v10;
    }
  }


}