#pragma once

#include <renderwidget.hh>
#include <valkyrie/graphics/vkshaderattributeid.hh>


struct iTexture2D;
struct iShader;
struct iVertexDeclaration;
struct iVertexBuffer;
struct iIndexBuffer;
class TextureEditorRenderWidget : public RenderWidget
{
public:
  TextureEditorRenderWidget(QWidget *parent);
  ~TextureEditorRenderWidget();

  void SetTexture(iTexture2D *texture);
  void SetLOD(int lod);
  void SetRed(bool red);
  void SetGreen(bool green);
  void SetBlue(bool blue);
  void SetAlpha(bool alpha);

protected:
  void initializeGL();
  void paintGL();

private:
  iTexture2D *m_texture;


  iShader *m_shader;
  iVertexDeclaration *m_vertexDeclaration;
  iVertexBuffer *m_vertexBuffer;
  iIndexBuffer *m_indexBuffer;

  vkShaderAttributeID m_idOffset;
  vkShaderAttributeID m_idScale;
  vkShaderAttributeID m_idDiffuse;
  vkShaderAttributeID m_idLOD;
  vkShaderAttributeID m_idColorTrans;

  float m_lod;
  bool m_red;
  bool m_green;
  bool m_blue;
  bool m_alpha;

};