

#include <Valkyrie/Engine.hh>
#include <Valkyrie/Window/IKeyboard.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Core/ResourceManager.hh>
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


  IShader *shader = vkResourceManager::Get()->GetOrLoad<IShader>(vkResourceLocator("${shaders}/solid_gbuffer.xml"));

  float vertexBuffer[] = {
    -0.5f, +0.5f, 0.0f, 1.0f,
    +0.5f, +0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
    +0.5f, -0.5f, 0.0f, 1.0f,
  };

  float colorBuffer[] = {
    0.25f, 0.75f, 0.0f, 1.0f,
    0.75f, 0.75f, 0.0f, 1.0f,
    0.25f, 0.25f, 0.0f, 1.0f,
    0.75f, 0.25f, 0.0f, 1.0f,
  };

  unsigned short indexBuffer[] = {
    0, 1, 2, 1, 3, 2,
    0, 2, 1, 1, 2, 3,
  };

  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
    vkVertexElement(eVST_Color, eDT_Float, 4, 0, sizeof(float) * 4, 1),
    vkVertexElement()
  };

  IVertexBuffer *vb = m_renderer->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  IVertexBuffer *cb = m_renderer->CreateVertexBuffer(sizeof(colorBuffer), colorBuffer, eBDM_Static);
  IIndexBuffer *ib = m_renderer->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  IVertexDeclaration *vd = m_renderer->CreateVertexDeclaration(elements);

  vkMesh *mesh = new vkMesh();
  mesh->SetIndexType(eDT_UnsignedShort);
  mesh->SetPrimitiveType(ePT_Triangles);
  mesh->SetVertexDeclaration(vd);
  mesh->AddVertexBuffer(vb);
  mesh->AddVertexBuffer(cb);
  mesh->AddIndexBuffer(ib, 12);

  vkMaterial *material = new vkMaterial();
  material->SetShader(eRP_GBuffer, shader);

  const IKeyboard *keyboard = m_window->GetKeyboard();

  vkShaderAttributeID attrib("Alt");

  float v = 0.0f;
  while (true)
  {
    m_window->UpdateEvents();
    if (keyboard->IsKeyPressed(eK_Esc))
    {
      break;
    }

    m_renderer->Clear();

    if (material->Bind(m_renderer, eRP_GBuffer))
    {
      /*
      m_renderer->SetShader(shader);
      IShaderAttribute *a = shader->GetAttribute(attrib);
      if (a)
      {
        shader->GetAttribute(attrib)->Set((float)cos(v) * 0.1f, (float)sin(v) * 0.1f);
      }
      v += 0.01f;
      */
      mesh->Render(m_renderer);
    }
    m_window->Present();

  }

  return 0;
}