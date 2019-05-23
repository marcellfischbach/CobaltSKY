

#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/csengine.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/gl4rendertarget.hh>
#include <graphicsgl4/gl4indexbuffer.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4texture2d.hh>
#include <graphicsgl4/gl4vertexbuffer.hh>
#include <graphicsgl4/gl4vertexdeclaration.hh>
#include <editor/editor.hh>

#include <textureeditor/textureeditorrenderwidget.hh>
#include <QColor>
#include <qopengl.h>

const char* VertexShaderProgram =
"#version 330\n"
"in vec2 cs_Position;\n"
"in vec2 cs_TexCoord0;\n"
"uniform vec2 cs_Scale;\n"
"uniform vec2 cs_Offset;\n"
"out vec2 fragTexCoord;\n"
"void main ()\n"
"{\n"
"  gl_Position = vec4(cs_Offset + cs_Position * cs_Scale, 0.0, 1.0);"
"  fragTexCoord = cs_TexCoord0;\n"
"}\n"
;

const char *FragmentShaderProgram =
"#version 330\n"
"layout(location = 0) out vec4 cs_FragColor;\n"
"uniform sampler2D cs_Diffuse;\n"
"uniform mat4 cs_ColorTrans;"
"uniform float cs_LOD;\n"
"in vec2 fragTexCoord;\n"
"void main ()\n"
"{\n"
"  cs_FragColor = cs_ColorTrans * texture(cs_Diffuse, fragTexCoord, cs_LOD);\n"
//"  cs_FragColor.a = 1.0;\n"
"}\n";


TextureEditorRenderWidget::TextureEditorRenderWidget(QWidget *parent)
  : RenderWidget(parent)
  , m_texture(0)
  , m_shader(0)
  , m_vertexDeclaration(0)
  , m_vertexBuffer(0)
  , m_indexBuffer(0)
  , m_idOffset(cs::ShaderAttributeID("Offset"))
  , m_idScale(cs::ShaderAttributeID("Scale"))
  , m_idDiffuse(cs::ShaderAttributeID("Diffuse"))
  , m_idLOD(cs::ShaderAttributeID("LOD"))
  , m_idColorTrans(cs::ShaderAttributeID("ColorTrans"))
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

void TextureEditorRenderWidget::SetTexture(cs::Texture2DWrapper *texture)
{
  CS_SET(m_texture, texture);
}

void TextureEditorRenderWidget::initializeGL()
{
//  RenderWidget::initializeGL();


  cs::iGraphics *gr = csEng->GetRenderer();
  m_shader = gr->CreateShader(VertexShaderProgram, "", "", "", FragmentShaderProgram);

  m_shader->RegisterAttribute(m_idOffset);
  m_shader->RegisterAttribute(m_idScale);
  m_shader->RegisterAttribute(m_idDiffuse);

  float vertices[] = {
    -1.0f, -1.0f,   0.0f, 1.0f,
    -1.0f, 1.0f,    0.0f, 0.0f,
    1.0f, -1.0f,   1.0f, 1.0f,
    1.0f, 1.0f,    1.0f, 0.0f,
  };

  short indices[] = {
    0, 1, 3,
    0, 3, 2,
  };

  cs::VertexElement elements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 2, 0, 4.0f * sizeof(float), 0),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, 2.0f * sizeof(float), 4.0f * sizeof(float), 0),
    cs::VertexElement()
  };

  m_vertexDeclaration = gr->CreateVertexDeclaration(elements);
  m_vertexBuffer = gr->CreateVertexBuffer(sizeof(vertices), vertices, cs::eBDM_Static);
  m_indexBuffer = gr->CreateIndexBuffer(sizeof(indices), indices, cs::eBDM_Static);

}

void TextureEditorRenderWidget::paintGL()
{
  RenderWidget::paintGL();



  cs::iTexture2D *texture = m_texture->Get();

  float sx = (float)texture->GetWidth() / (float)width();
  float sy = (float)texture->GetHeight() / (float)height();

  cs::Vector2f o(-(1.0f - sx), (1.0f - sy));
  cs::Vector2f s(sx, sy);

  cs::iGraphics *gr = csEng->GetRenderer();
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
  cs::Matrix4f colorTrans(
    r, 0, 0, 0,
    0, g, 0, 0,
    0, 0, b, 0,
    atoc, atoc, atoc, a
    );


  cs::iShaderAttribute *attrOffset = m_shader->GetAttribute(m_idOffset);
  if (attrOffset)
  {
    attrOffset->Set(o);
  }
  cs::iShaderAttribute *attrScale = m_shader->GetAttribute(m_idScale);
  if (attrScale)
  {
    attrScale->Set(s);
  }
  cs::iShaderAttribute *attrLOD = m_shader->GetAttribute(m_idLOD);
  if (attrLOD)
  {
    attrLOD->Set(m_lod);
  }
  cs::iShaderAttribute *attrDiffuse = m_shader->GetAttribute(m_idDiffuse);
  if (attrDiffuse)
  {
    cs::eTextureUnit unit = gr->BindTexture(texture);
    attrDiffuse->Set(unit);
  }
  cs::iShaderAttribute *attrColorTrans = m_shader->GetAttribute(m_idColorTrans);
  if (attrColorTrans)
  {
    attrColorTrans->Set(colorTrans);
  }

  gr->SetBlendEnabled(true);
  gr->SetBlendMode(cs::eBM_SrcAlpha, cs::eBM_One);
  gr->SetColorMask(true, true, true, true);
  gr->Clear(true, cs::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));

  gr->RenderIndexed(cs::ePT_Triangles, 6, cs::eDT_UnsignedShort);

}
