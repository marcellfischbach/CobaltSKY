

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/Image.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/Scene/GeometryNode.hh>
#include <Valkyrie/Loaders/Loaders.hh>
#include <Valkyrie/Window/IKeyboard.hh>
#include <math.h>


vkEngine::vkEngine()
  : m_window (0)
  , m_renderer(0)
{

}

vkEngine *vkEngine::Get()
{
  static vkEngine engine;
  return &engine;
}

void vkEngine::SetWindow(IWindow *window)
{
  VK_SET(m_window, window);
}

void vkEngine::SetRenderer(IRenderer *renderer)
{
  VK_SET(m_renderer, renderer);
}

int vkEngine::Run()
{
  if (!m_window)
  {
    return -1;
  }

  RegisterLoaders();

  
  float vertexBuffer[] = {
    -10.0f, +10.0f, 0.0f, 1.0f,
    +10.0f, +10.0f, 0.0f, 1.0f,
    -10.0f, -10.0f, 0.0f, 1.0f,
    +10.0f, -10.0f, 0.0f, 1.0f,
  };

  float colorBuffer[] = {
    0.25f, 0.75f, 0.0f, 1.0f,
    0.75f, 0.75f, 0.0f, 1.0f,
    0.25f, 0.25f, 0.0f, 1.0f,
    0.75f, 0.25f, 0.0f, 1.0f,
  };

  float texCoordBuffer[] = {
    0.0f, 0.0f,
    2.0f, 0.0f,
    0.0f, 2.0f,
    2.0f, 2.0f,
  };

  unsigned short indexBuffer[] = {
    0, 1, 2, 1, 3, 2,
    0, 2, 1, 1, 2, 3,
  };


  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Color, eDT_Float, 4, 0, sizeof(float) * 4, 1),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 0, sizeof(float) * 2, 2),
    vkVertexElement()
  };

  IVertexBuffer *vb = m_renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *cb = m_renderer->CreateVertexBuffer(sizeof(colorBuffer), colorBuffer, eBDM_Static);
  IVertexBuffer *tb = m_renderer->CreateVertexBuffer(sizeof(texCoordBuffer), texCoordBuffer, eBDM_Static);
  IIndexBuffer *ib = m_renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = m_renderer->CreateVertexDeclaration(elements);

  vkMesh *mesh = new vkMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(cb);
  mesh->AddVertexBuffer(tb);
  mesh->AddIndexBuffer(ib, 12);


  vkMaterialInstance *materialInstance = vkResourceManager::Get()->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", "FieldStone"));

  vkGeometryNode *geometryNode = new vkGeometryNode();
  geometryNode->SetMesh(mesh);
  geometryNode->SetMaterial(materialInstance);

  float asp = 768.0f / 1366.0f;
  vkMatrix4f projMatrix;
  projMatrix.SetPerspective(-1.0f, 1.0f, -asp, asp, 1.0f, 1024.0f);
  m_renderer->SetProjectionMatrix(projMatrix);

  vkMatrix4f viewMatrix;
  vkMatrix4f modelMatrix;

  float v = 0.0f;
  float m = 0.0f;

  const IKeyboard *keyboard = m_window->GetKeyboard();

  while (true)
  {
    m_window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }

    m_renderer->Clear();


    viewMatrix.SetLookAt(vkVector3f(10.0f * (float)cos(v), 10.0f * (float)sin(v), 0.5f),
                         vkVector3f(0.0f, 0.0f, 0.0f), 
                         vkVector3f(0.0f, 0.0f, 1.0f));
    v += 0.00001f;


    m_renderer->SetViewMatrix(viewMatrix);

    vkMatrix4f mm = geometryNode->GetMatrix();
    mm.SetRotationX(m);
    geometryNode->SetMatrix(mm);
    m += 0.00f;

    geometryNode->UpdateStates();

    geometryNode->Render(m_renderer, eRP_GBuffer);
    m_window->Present();

  }

  return 0;
}



void vkEngine::RegisterLoaders()
{
  vkResourceManager *mgr = vkResourceManager::Get();
  vkLoaders::Register(mgr);


}
