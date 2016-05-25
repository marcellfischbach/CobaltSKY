
#pragma once
#include <GL/glew.h>
#include <qopenglwidget.h>
#include <Valkyrie/Math/Color.hh>


struct IGraphics;
struct ISampler;
struct ITexture2D;
struct IVertexBuffer;
struct IIndexBuffer;
struct IVertexDeclaration;
struct IShader;
struct IShaderAttribute;


namespace texture
{

class PreviewWidget : public QOpenGLWidget
{
public:
  PreviewWidget(QWidget *parent = 0);
  ~PreviewWidget();

  void SetTexture(ITexture2D* texture);

  void SetRed(bool red);
  void SetGreen(bool green);
  void SetBlue(bool blue);
  void SetAlpha(bool alpha);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);


protected:


private:

  ITexture2D *m_texture;
  IGraphics *m_graphics;
  ISampler *m_sampler;
  IVertexBuffer *m_vertexBuffer;
  IIndexBuffer *m_indexBuffer;
  IVertexDeclaration *m_vertexDeclaration;
  IShader *m_shaderColor;
  IShaderAttribute *m_attrTextureScaleColor;
  IShaderAttribute *m_attrDiffuseColor;
  IShaderAttribute *m_attrColorScaleColor;
  IShaderAttribute *m_attrInsetColor;

  IShader *m_shaderAlpha;
  IShaderAttribute *m_attrTextureScaleAlpha;
  IShaderAttribute *m_attrDiffuseAlpha;
  IShaderAttribute *m_attrInsetAlpha;

  bool m_maskRed;
  bool m_maskGreen;
  bool m_maskBlue;
  bool m_maskAlpha;
};

}