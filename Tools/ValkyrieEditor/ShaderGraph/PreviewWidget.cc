

#include <ShaderGraph/PreviewWidget.hh>
#include <SceneWidget/Camera.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/LightState.hh>
#include <Valkyrie/Entity/MeshState.hh>
#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Graphics/Camera.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IRenderTarget.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/Light.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <Editor.hh>
#include <qopenglcontext.h>
#include <math.h>

namespace shadergraph
{


PreviewWidget::PreviewWidget(QWidget *parent)
  : scenewidget::SceneWidget(parent)
  , m_material(0)
  , m_materialInstance(0)
  , m_orbitCamera(0)
  , m_lightRotating (false)
  , m_lightRotH(0.0f)
  , m_lightRotV(0.0f)
{
}

PreviewWidget::~PreviewWidget()
{
  delete m_orbitCamera;
}

QSize PreviewWidget::sizeHint() const
{
  return QSize(255, 255);
} 

void PreviewWidget::SetMaterial(vkMaterial *material)
{
  VK_SET(m_material, material);
  if (m_materialInstance)
  {
    m_materialInstance->SetMaterial(material);
  }
}

void PreviewWidget::initializeGL()
{
  scenewidget::SceneWidget::initializeGL();

  CreateScene();

  m_orbitCamera = new scenewidget::OrbitCamera(m_camera, 3.1415f / 4.0f, -3.1415f / 4.0f);
  AddEventListener(m_orbitCamera);

}

void PreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
  if (m_lightRotating)
  {
    QPoint diff = event->pos() - m_lastPoint;
    m_lastPoint = event->pos();


    m_lightRotH -= (float)(diff.x()) / 100.0f;
    m_lightRotV -= (float)(diff.y()) / 100.0f;
    printf ("LightRotating: %f %f\n", m_lightRotH, m_lightRotV);
    fflush(stdout);
    UpdateLight();
  }
  scenewidget::SceneWidget::mouseMoveEvent(event);
}

void PreviewWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton)
  {
    m_lightRotating = true;
    m_lastPoint = event->pos();
  }

  scenewidget::SceneWidget::mousePressEvent(event);
}

void PreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton)
  {
    m_lightRotating = false;
  }
  scenewidget::SceneWidget::mouseReleaseEvent(event);
}

void PreviewWidget::UpdateLight()
{

  vkMatrix4f MH, MV, M;
  MH.SetRotationZ(m_lightRotH);
  MV.SetRotationX(m_lightRotV);

  vkMatrix4f::Mult(MH, MV, M);

  vkVector3f dir = M.GetZAxis(dir);
  vkVector3f::Mul(dir, -1.0f, dir);

  m_light->SetDirection(dir);
  repaint();
}

vkEntityScene* PreviewWidget::CreateScene()
{
  m_materialInstance = new vkMaterialInstance();
  m_materialInstance->SetMaterial(m_material);

  vkEntityScene *entityScene = GetScene();
  vkSubMesh *planeSubMesh = CreatePlaneMesh(20.0f, 0.0f);
  vkSubMesh *sphereSubMesh = CreateSphereMesh(10.0, 16, 32);

  vkMesh *mesh = new vkMesh();
  mesh->AddMesh(sphereSubMesh);
  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();

  m_staticMeshState = new vkStaticMeshState();
  m_staticMeshState->SetMesh(mesh);
  m_staticMeshState->SetMaterial(m_materialInstance, 0);
  m_staticMeshState->SetCastShadow(true);

  vkEntity *planeEntity = new vkEntity();
  planeEntity->SetRootState(m_staticMeshState);
  planeEntity->AddState(m_staticMeshState);

  entityScene->AddEntity(planeEntity);



  //
  // create the light for the scene
  m_light = new vkDirectionalLight();
  m_light->SetColor(vkColor4f(1.0f, 1.0f, 1.0f));
  m_light->SetArbDirection(vkVector3f(-1.0f, -1.0f, -0.5f));
  m_light->SetCastShadow(false);
  m_light->SetShadowIntensity(0.0f);

  vkLightState *directionalLightState = new vkLightState();
  directionalLightState->SetLight(m_light);

  vkEntity *directionalLightEntity = new vkEntity();
  directionalLightEntity->SetRootState(directionalLightState);
  directionalLightEntity->AddState(directionalLightState);
  entityScene->AddEntity(directionalLightEntity);



  return entityScene;
}





vkSubMesh* PreviewWidget::CreatePlaneMesh(float size, float height)
{
  float s = size;
  float vertexBuffer[] = {
    -s, -s, height, 1.0f,
    -s,  s, height, 1.0f,
    s, -s, height, 1.0f,
    s,  s, height, 1.0f,
    s, -s, height, 1.0f,
    s,  s, height, 1.0f,
    -s, -s, height, 1.0f,
    -s,  s, height, 1.0f,
  };

  float normalBuffer[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
  };

  float texCoordBuffer[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
  };

  unsigned short indexBuffer[] = {
    0, 1, 3, 0, 3, 2,
    4, 5, 7, 4, 7, 6,
  };


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 2),
    vkVertexElement()
  };

  IVertexBuffer *vb = m_graphics->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *nb = m_graphics->CreateVertexBuffer(sizeof(normalBuffer), normalBuffer, eBDM_Static);
  IVertexBuffer *tb = m_graphics->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  IIndexBuffer *ib = m_graphics->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = m_graphics->CreateVertexDeclaration(elements);

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(-s, -s, 0));
  bbox.Add(vkVector3f(s, s, 0));
  bbox.Finish();

  vkSubMesh *mesh = new vkSubMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, sizeof(indexBuffer) / sizeof(indexBuffer[0]));
  mesh->SetBoundingBox(bbox);

  vb->Release();
  nb->Release();
  tb->Release();
  vd->Release();

  return mesh;
}

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

vkSubMesh* PreviewWidget::CreateSphereMesh(float radius, int numH, int numV)
{
  vkSize num = numH * (numV+1);
  vkVector3f *pos = new vkVector3f [num];
  vkVector3f *nor = new vkVector3f [num];
  vkVector3f *tan = new vkVector3f [num];
  vkVector3f *bin = new vkVector3f [num];
  vkVector2f *txt = new vkVector2f [num];


  unsigned i=0;
  for (unsigned h=0; h<numH; ++h)
  {
    float fH = (float)h / (float)(numH-1);
    float angleH = M_PI / 2.0f - fH * M_PI;
    for (unsigned v=0; v<=numV; ++v, ++i)
    {
      float fV = (float)v / (float)numV;
      float angleV = fV * M_PI * 2.0f;

      nor[i].Set(cos(angleH) * cos(angleV),
                 cos(angleH) * sin(angleV),
                 sin(angleH));
      tan[i].Set(sin(angleV),
                 cos(angleV),
                 0.0f);
      vkVector3f::Cross(tan[i], nor[i], bin[i]);
      vkVector3f::Mul(nor[i], radius, pos[i]);

      txt[i].Set(fV, fH);
    }
  }
  fflush(stdout);

  unsigned numIndex = (numH-1) * numV * 2 * 3;

  unsigned short *indexBuffer = new unsigned short[numIndex];
  unsigned short *iptr = indexBuffer;
  for (unsigned h=0; h<(numH-1); ++h)
  {
    for (unsigned v=0; v<numV; ++v)
    {
      unsigned short i00 = h * (numV+1) + v;
      unsigned short i01 = i00 + 1;
      unsigned short i10 = i00 + (numV+1);
      unsigned short i11 = i10 + 1;
      *iptr++ = i00;
      *iptr++ = i01;
      *iptr++ = i10;

      *iptr++ = i10;
      *iptr++ = i01;
      *iptr++ = i11;
    }
  }
  fflush(stdout);


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 3, 0, sizeof(float) * 3, 0),
    vkVertexElement(eVST_Normal, eDT_Float, 3, 0, sizeof(float) * 3, 1),
    vkVertexElement(eVST_Tangent, eDT_Float, 3, 0, sizeof(float) * 3, 2),
    vkVertexElement(eVST_BiNormal, eDT_Float, 3, 0, sizeof(float) * 3, 3),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 4),
    vkVertexElement()
  };

  IVertexBuffer *vb = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, pos, eBDM_Static);
  IVertexBuffer *nb = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, nor, eBDM_Static);
  IVertexBuffer *tab = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, tan, eBDM_Static);
  IVertexBuffer *bb = m_graphics->CreateVertexBuffer(sizeof(vkVector3f) * num, bin, eBDM_Static);
  IVertexBuffer *tb = m_graphics->CreateVertexBuffer(sizeof(vkVector2f) * num, txt, eBDM_Static);
  IIndexBuffer *ib = m_graphics->CreateIndexBuffer(sizeof(unsigned short) * numIndex, indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = m_graphics->CreateVertexDeclaration(elements);

  vkBoundingBox bbox;
  bbox.Add(vkVector3f(-radius, -radius, -radius));
  bbox.Add(vkVector3f(radius, radius, radius));
  bbox.Finish();

  vkSubMesh *mesh = new vkSubMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(nb);
  mesh->AddVertexBuffer(tab);
  mesh->AddVertexBuffer(bb);
  mesh->AddVertexBuffer(tb);
  mesh->SetIndexBuffer(ib, numIndex);
  mesh->SetBoundingBox(bbox);

  vb->Release();
  nb->Release();
  tb->Release();
  vd->Release();

  return mesh;
}



}
