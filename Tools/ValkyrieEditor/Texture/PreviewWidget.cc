
#include <Texture/PreviewWidget.hh>
#include <qopenglcontext.h>
#include <Editor.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <GraphicsGL4/Shader.hh>

namespace texture
{

PreviewWidget::PreviewWidget(QWidget *parent)
  : QOpenGLWidget(parent)
  , m_texture(0)
{

}

PreviewWidget::~PreviewWidget()
{

}

void PreviewWidget::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());

  Editor::Get().RequestGraphics();


  m_graphics = vkEngine::Get()->GetRenderer();
  m_graphics->ResetDefaults();


  //
  // the sampler that is used for sampling the color buffer
  m_sampler = m_graphics->CreateSampler();
  m_sampler->SetFilter(eFM_MinMagNearest);




  float vertexBuffer[] = {
    0.0f, 1.0f,     0.0f, 1.0f,
    0.0f, 0.0f,     0.0f, 0.0f,
    1.0f, 1.0f,     1.0f, 1.0f,
    1.0f, 0.0f,     1.0f, 0.0f,
  };

  short indexBuffer[] = {
    0, 1, 3,
    0, 3, 2,
  };

  vkVertexElement vertexElements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 2, 0, 16, 0),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 8, 16, 0),
    vkVertexElement(),
  };

  m_vertexBuffer = m_graphics->CreateVertexBuffer(sizeof(vertexBuffer), vertexBuffer, eBDM_Static);
  m_indexBuffer = m_graphics->CreateIndexBuffer(sizeof(indexBuffer), indexBuffer, eBDM_Static);
  m_vertexDeclaration = m_graphics->CreateVertexDeclaration(vertexElements);


  

  vkString vertexShaderCode = "\n"
    "#version 330\n"
    "\n"
    "uniform vec2 vk_TextureScale;\n"
    "\n"
    "in vec4 vk_Position;\n"
    "in vec2 vk_TexCoord0;\n"
    "\n"
    "out vec2 inFragTexCoord;\n"
    "\n"
    "void main ()\n"
    "{\n"
    "  gl_Position = vec4 (vec2(-1, 1) + vk_Position.xy * vk_TextureScale, 0.0, 1.0);\n"
    "  inFragTexCoord = vk_TexCoord0;\n"
    "}\n"
    "\n";
  vkString fragmentShaderColorCode = "\n"
    "#version 330\n"
    "\n"
    "layout(location = 0) out vec4 vk_FragColor;\n"
    "\n"
    "uniform sampler2D vk_Diffuse;\n"
    "uniform vec4 vk_ColorScale;\n"
    "\n"
    "in vec2 inFragTexCoord;\n"
    "\n"
    "void main ()\n"
    "{\n"
    "  vk_FragColor = texture(vk_Diffuse, inFragTexCoord) * vk_ColorScale;\n"
    "}\n"
    "\n";
  vkString fragmentShaderAlphaCode = "\n"
    "#version 330\n"
    "\n"
    "layout(location = 0) out vec4 vk_FragColor;\n"
    "\n"
    "uniform sampler2D vk_Diffuse;\n"
    "\n"
    "in vec2 inFragTexCoord;\n"
    "\n"
    "void main ()\n"
    "{\n"
    "  vk_FragColor = vec4(texture(vk_Diffuse, inFragTexCoord).aaa, 1.0);\n"
    "}\n"
    "\n";

  m_shaderColor = m_graphics->CreateShader(
    vertexShaderCode,
    "",
    "",
    "",
    fragmentShaderColorCode);
  m_attrDiffuseColor = m_shaderColor->GetAttribute(vkShaderAttributeID("Diffuse"));
  m_attrTextureScaleColor = m_shaderColor->GetAttribute(vkShaderAttributeID("TextureScale"));
  m_attrColorScaleColor = m_shaderColor->GetAttribute(vkShaderAttributeID("ColorScale"));


  m_shaderAlpha = m_graphics->CreateShader(
    vertexShaderCode,
    "",
    "",
    "",
    fragmentShaderAlphaCode);
  m_attrDiffuseAlpha = m_shaderAlpha->GetAttribute(vkShaderAttributeID("Diffuse"));
  m_attrTextureScaleAlpha = m_shaderAlpha->GetAttribute(vkShaderAttributeID("TextureScale"));
}

void PreviewWidget::SetTexture(ITexture2D *texture)
{
  VK_SET(m_texture, texture);
}

void PreviewWidget::paintGL()
{
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_texture)
  {
    if (!(m_maskRed | m_maskGreen | m_maskBlue))
    {
      m_graphics->SetShader(m_shaderAlpha);
    }
    else
    {
      m_graphics->SetShader(m_shaderColor);
    }
    m_graphics->ResetDefaults();
    vkTextureUnit tu = m_graphics->BindTexture(m_texture);
    m_graphics->SetVertexDeclaration(m_vertexDeclaration);
    m_graphics->SetVertexBuffer(0, m_vertexBuffer);
    m_graphics->SetIndexBuffer(m_indexBuffer);
    m_graphics->SetBlendEnabled(true);
    m_graphics->SetBlendMode(eBM_SrcAlpha, eBM_InvSrcAlpha);

    if (!(m_maskRed | m_maskGreen | m_maskBlue))
    {
      if (m_attrDiffuseAlpha)
      {
        m_attrDiffuseAlpha->Set((vkInt32)tu);
      }
      if (m_attrTextureScaleAlpha)
      {
        m_attrTextureScaleAlpha->Set(vkVector2f((float)m_texture->GetWidth() / (float)width(), -(float)m_texture->GetHeight() / (float)height()));
      }
    }
    else
    {
      if (m_attrDiffuseColor)
      {
        m_attrDiffuseColor->Set((vkInt32)tu);
      }
      if (m_attrTextureScaleColor)
      {
        m_attrTextureScaleColor->Set(vkVector2f((float)m_texture->GetWidth() / (float)width(), -(float)m_texture->GetHeight() / (float)height()));
      }
      if (m_attrColorScaleColor)
      {
        vkColor4f colorScale = vkColor4f(m_maskRed ? 1.0f : 0.0f,
                                         m_maskGreen ? 1.0f : 0.0f,
                                         m_maskBlue ? 1.0f : 0.0f,
                                         m_maskAlpha ? 1.0f : 0.0f);

        m_attrColorScaleColor->Set(colorScale);
      }
    }



    m_graphics->RenderIndexed(ePT_Triangles, 6, eDT_UnsignedShort);
    
  }
}

void PreviewWidget::resizeGL(int width, int height)
{
  if (m_texture)
  {

  }
  glViewport(100, 100, 512, 512);

}



}