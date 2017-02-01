

#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/vkengine.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/gl4rendertarget.hh>
#include <graphicsgl4/gl4indexbuffer.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <GraphicsGL4/gl4texture2d.hh>
#include <graphicsgl4/gl4vertexbuffer.hh>
#include <graphicsgl4/gl4vertexdeclaration.hh>
#include <editor.hh>

#include <textureeditor/textureeditorrenderwidget.hh>
#include <QColor>
#include <qopengl.h>

const char* VertexShaderProgram =
"#version 330\n"
"in vec2 vk_Position;\n"
"in vec2 vk_TexCoord0;\n"
"uniform vec2 vk_Scale;\n"
"uniform vec2 vk_Offset;\n"
"out vec2 fragTexCoord;\n"
"void main ()\n"
"{\n"
"  gl_Position = vec4(vk_Offset + vk_Position * vk_Scale, 0.0, 1.0);"
"  fragTexCoord = vk_TexCoord0;\n"
"}\n"
;

const char *FragmentShaderProgram =
"#version 330\n"
"layout(location = 0) out vec4 vk_FragColor;\n"
"uniform sampler2D vk_Diffuse;\n"
"uniform mat4 vk_ColorTrans;"
"uniform float vk_LOD;\n"
"in vec2 fragTexCoord;\n"
"void main ()\n"
"{\n"
"  vk_FragColor = vk_ColorTrans * texture(vk_Diffuse, fragTexCoord, vk_LOD);\n"
"}\n";


TextureEditorRenderWidget::TextureEditorRenderWidget(QWidget *parent)
  : RenderWidget(parent)
  , m_texture(0)
  , m_shader(0)
  , m_vertexDeclaration(0)
  , m_vertexBuffer(0)
  , m_indexBuffer(0)
  , m_idOffset(vkShaderAttributeID("Offset"))
  , m_idScale(vkShaderAttributeID("Scale"))
  , m_idDiffuse(vkShaderAttributeID("Diffuse"))
  , m_idLOD(vkShaderAttributeID("LOD"))
  , m_idColorTrans(vkShaderAttributeID("ColorTrans"))
  , m_lod(0.0f)
  , m_red(true)
  , m_green(true)
  , m_blue(true)
  , m_alpha(true)

{

}

TextureEditorRenderWidget::~TextureEditorRenderWidget()
{

}

void TextureEditorRenderWidget::SetRed(bool red)
{
  m_red = red;
  repaint();
}
void TextureEditorRenderWidget::SetGreen(bool green)
{
  m_green = green;
  repaint();
}
void TextureEditorRenderWidget::SetBlue(bool blue)
{
  m_blue = blue;
  repaint();
}
void TextureEditorRenderWidget::SetAlpha(bool alpha)
{
  m_alpha = alpha;
  repaint();
}
void TextureEditorRenderWidget::SetLOD(int lod)
{
  m_lod = lod;
  repaint();
}

void TextureEditorRenderWidget::SetTexture(iTexture2D *texture)
{
  VK_SET(m_texture, texture);
}

void TextureEditorRenderWidget::initializeGL()
{
  RenderWidget::initializeGL();


  iGraphics *gr = vkEng->GetRenderer();
  m_shader = gr->CreateShader(VertexShaderProgram, "", "", "", FragmentShaderProgram);

  m_shader->RegisterAttribute(m_idOffset);
  m_shader->RegisterAttribute(m_idScale);
  m_shader->RegisterAttribute(m_idDiffuse);

  float vertices[] = {
    -1.0f, -1.0f,   0.0f, 0.0f,
    -1.0f, 1.0f,    0.0f, 1.0f,
    1.0f, -1.0f,   1.0f, 0.0f,
    1.0f, 1.0f,    1.0f, 1.0f,
  };

  short indices[] = {
    0, 1, 3,
    0, 3, 2,
  };

  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 2, 0, 4.0f * sizeof(float), 0),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 2.0f * sizeof(float), 4.0f * sizeof(float), 0),
    vkVertexElement()
  };

  m_vertexDeclaration = gr->CreateVertexDeclaration(elements);
  m_vertexBuffer = gr->CreateVertexBuffer(sizeof(vertices), vertices, eBDM_Static);
  m_indexBuffer = gr->CreateIndexBuffer(sizeof(indices), indices, eBDM_Static);

}

void TextureEditorRenderWidget::paintGL()
{
  RenderWidget::paintGL();



  float sx = (float)m_texture->GetWidth() / (float)width();
  float sy = (float)m_texture->GetHeight() / (float)height();

  vkVector2f o(-(1.0f - sx), (1.0f - sy));
  vkVector2f s(sx, sy);

  iGraphics *gr = vkEng->GetRenderer();
  gr->ResetDefaults();
  gr->SetShader(m_shader);

  gr->SetVertexDeclaration(m_vertexDeclaration);
  gr->SetVertexBuffer(0, m_vertexBuffer);
  gr->SetIndexBuffer(m_indexBuffer);


  float r = m_red ? 1.0f : 0.0f;
  float g = m_green ? 1.0f : 0.0f;
  float b = m_blue ? 1.0f : 0.0f;
  float a = m_alpha ? 1.0f : 0.0f;
  float atoc = !m_red && !m_green && !m_blue && m_alpha;
  vkMatrix4f colorTrans(
    r, 0, 0, 0,
    0, g, 0, 0,
    0, 0, b, 0,
    atoc, atoc, atoc, a
    );

  iShaderAttribute *attrOffset = m_shader->GetAttribute(m_idOffset);
  if (attrOffset)
  {
    attrOffset->Set(o);
  }
  iShaderAttribute *attrScale = m_shader->GetAttribute(m_idScale);
  if (attrScale)
  {
    attrScale->Set(s);
  }
  iShaderAttribute *attrLOD = m_shader->GetAttribute(m_idLOD);
  if (attrLOD)
  {
    attrLOD->Set(m_lod);
  }
  iShaderAttribute *attrDiffuse = m_shader->GetAttribute(m_idDiffuse);
  if (attrDiffuse)
  {
    vkTextureUnit unit = gr->BindTexture(m_texture);
    attrDiffuse->Set(unit);
  }
  iShaderAttribute *attrColorTrans = m_shader->GetAttribute(m_idColorTrans);
  if (attrColorTrans)
  {
    attrColorTrans->Set(colorTrans);
  }

  gr->Clear(true, vkVector4f(0.0f, 0.0f, 0.5f, 1.0f));

  gr->RenderIndexed(ePT_Triangles, 6, eDT_UnsignedShort);

}